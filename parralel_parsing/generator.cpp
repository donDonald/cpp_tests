#include <iostream>

#include <atomic>
#include <thread>
#include <fstream>
#include <filesystem>
#include <random>
#include <unordered_map>
#include <mutex>
#include <functional>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>

using namespace boost::program_options;

using namespace std;
static const string BIN_NAME = "generator";

using FileRecordsCount_t = uint32_t;
using RecordValue_t = uint16_t;
using UpdateResultsFoo = std::function<void(unordered_map<RecordValue_t, uint64_t>&)>;




class Worker
{
public:
    Worker(string& dstFolder,
           std::atomic<int64_t>& numFiles,
           string& filePrefix,
           FileRecordsCount_t& minSize,
           FileRecordsCount_t& maxSize,
           UpdateResultsFoo updateResultsFoo)
        : _dstFolder(dstFolder)
        , _numFiles(numFiles)
        , _filePrefix(filePrefix)
        , _minSize(minSize)
        , _maxSize(maxSize)
        , _resultsMaxSize(numeric_limits<RecordValue_t>::max())
        , _results()
        , _updateResultsFoo(updateResultsFoo)
        {
            _results.reserve(_resultsMaxSize);
            _thread = thread(&Worker::foo, this);
        }

    void wait()
    {
        _thread.join();
    }

private:
    void foo()
    {
        std::thread::id this_id = std::this_thread::get_id();
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<FileRecordsCount_t> fileRecordsCountDistrimution(_minSize, _maxSize);
        uniform_int_distribution<RecordValue_t> valueDistrimution(numeric_limits<RecordValue_t>::min(),
                                                                  numeric_limits<RecordValue_t>::max());
        filesystem::path curPath = filesystem::current_path();
        curPath.append(_dstFolder);
        while(1)
        {
            auto index = _numFiles.fetch_sub(1);
            if(index > 0)
            {
                _results.clear();
                string fileName = _filePrefix + "_" + to_string(index) + ".csv";
                filesystem::path p(curPath);
                p.append(fileName);
                ofstream ofs(p.string(), ofstream::out);
                FileRecordsCount_t count = fileRecordsCountDistrimution(gen);
                while(count > 0) 
                {
                    RecordValue_t v = valueDistrimution(gen);
                    ofs << v << "," << endl;
                    --count;
                    ++_results[v];
                    if(_results.size() >= _resultsMaxSize)
                    {
                        flushResults();
                    }
                }
                ofs.close();
                flushResults();
            }
            else
                break;
        }
    }

    void flushResults()
    {
        if(!_results.empty())
        {
            _updateResultsFoo(_results);
            _results.clear();
        }
    }

    string&               _dstFolder;
    std::atomic<int64_t>& _numFiles;
    string&               _filePrefix;
    FileRecordsCount_t&   _minSize;
    FileRecordsCount_t&   _maxSize;
    thread                _thread;
    int                   _resultsMaxSize;
    unordered_map<RecordValue_t, uint64_t> _results;
    UpdateResultsFoo      _updateResultsFoo;
};




class Generator
{
public:
    Generator(const string&      dstFolder,
              int64_t            numFiles,
              const string&      filePrefix,
              FileRecordsCount_t minSize,
              FileRecordsCount_t maxSize,
              const string&      indexFile)
        : _dstFolder(dstFolder)
        , _numFiles(numFiles)
        , _filePrefix(filePrefix)
        , _minSize(minSize)
        , _maxSize(maxSize)
        , _indexFile(indexFile)
        , _resultsMaxSize(numeric_limits<RecordValue_t>::max())
        , _results()
        {
            _results.reserve(_resultsMaxSize);
            if (!std::filesystem::is_directory(_dstFolder))
                throw invalid_argument(_dstFolder + " folder doesn't exists");
        }

    void run()
    {
        auto nproc = std::thread::hardware_concurrency();
        std::vector<std::unique_ptr<Worker>> workers;
        while (nproc > 0)
        {
            UpdateResultsFoo update = std::bind(&Generator::updateResults, this, placeholders::_1);
            auto worker = make_unique<Worker>(_dstFolder,
                                              _numFiles,
                                              _filePrefix,
                                              _minSize,
                                              _maxSize,
                                              update);
            workers.emplace(workers.end(), std::move(worker));
            --nproc;
        }

        for(auto& worker : workers)
        {
            worker->wait();
        }

        saveResults();
    }

private:
    void updateResults(unordered_map<RecordValue_t, uint64_t>& results)
    {
        lock_guard<mutex> lock(_mutex);
        for(const auto& [value, count]: results)
        {
            _results[value] += count;
        }
    }

    void saveResults()
    {
        // Turn unordered results into ordered ones
        map<RecordValue_t, uint64_t> orderedResults;
        for(const auto& [value, count]: _results)
            orderedResults[value] = count;

        filesystem::path curPath = filesystem::current_path();
        curPath.append(_dstFolder);
        curPath.append(_indexFile);

        ofstream ofs(curPath.string(), ofstream::out);
        uint64_t valuesTotalCount = 0;

        for(const auto& [value, count]: orderedResults)
        {
            valuesTotalCount += count;
            ofs << value << ":" << count << ","<< endl;
        }
        ofs << "values totally:" << valuesTotalCount << endl;
        ofs.close();
    }

    string               _dstFolder;
    std::atomic<int64_t> _numFiles;
    string               _filePrefix;
    FileRecordsCount_t   _minSize;
    FileRecordsCount_t   _maxSize;
    string               _indexFile;
    mutex                _mutex;
    int                  _resultsMaxSize;
	unordered_map<RecordValue_t, uint64_t> _results;
};




void print_help(const options_description& od)
{
    cout << "Generate a bunch of files containing test data" << std::endl
         << "example: " << BIN_NAME 
         << " --dst-folder=output --num-files=100 --file-prefix=data --min-size=100 --max-size=1000000 --index-file=data.index"
         << std::endl << od;
}




int main(int argc, char* argv[])
{
    unique_ptr<Generator> generator;
    options_description general("Options");

    try
    {
        general.add_options()
            ("help", "produce a help message")
            ("dst-folder", value<string>(), "destination folder to strore generated files to")
            ("num-files", value<int64_t>(), "amount of files to be generated")
            ("file-prefix", value<string>(), "prefix for files to be generated")
            ("min-size", value<FileRecordsCount_t>(), "minimum of records per file")
            ("max-size", value<FileRecordsCount_t>(), "maximum of records per file")
            ("index-file", value<string>(), "index file containing overall generation results, i.e <value>:<count> pairs")
            ;

        variables_map vm;
        store(parse_command_line(argc, argv, general), vm);

        if (vm.count("help")) 
        {
            print_help(general);
            return 0;
		}

        string             dstFolder;
        int64_t            numFiles;
        string             filePrefix;
        FileRecordsCount_t minSize;
        FileRecordsCount_t maxSize;
        string             indexFile;

        if (!vm.count("dst-folder"))
            throw std::invalid_argument("missing dst-folder");
        dstFolder = vm["dst-folder"].as<string>();

        if (!vm.count("num-files"))
            throw std::invalid_argument("missing num-files");
        numFiles = vm["num-files"].as<int64_t>();

        if (!vm.count("file-prefix"))
            throw std::invalid_argument("missing file-prefix");
        filePrefix = vm["file-prefix"].as<string>();

        if (!vm.count("min-size"))
            throw std::invalid_argument("missing min-size");
        minSize = vm["min-size"].as<FileRecordsCount_t>();

        if (!vm.count("max-size"))
            throw std::invalid_argument("missing max-size");
        maxSize = vm["max-size"].as<FileRecordsCount_t>();

        if (!vm.count("index-file"))
            throw std::invalid_argument("missing file-index");
        indexFile = vm["index-file"].as<string>();

        generator = make_unique<Generator>(dstFolder,
                                           numFiles,
                                           filePrefix,
                                           minSize,
                                           maxSize,
                                           indexFile);

//      cout << "dst-folder:" << dstFolder << endl;
//      cout << "num-files:" << numFiles << endl;
//      cout << "file-prefix:" << filePrefix << endl;
//      cout << "min-size:" << minSize << endl;
//      cout << "max-size:" << maxSize << endl;
    }
    catch(std::invalid_argument& e)
    {
        print_help(general);
        return -1;
    }
    catch(std::exception& e)
    {
        cerr << e.what() << ", exiting"<< std::endl;
        return -1;
    }

    generator->run();

    return 0;
}
