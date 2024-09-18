#include <iostream>

#include <atomic>
#include <thread>
#include <fstream>
#include <filesystem>
#include <random>
#include <unordered_map>
#include <mutex>
#include <functional>
#include <optional>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>

using namespace boost;
using namespace boost::program_options;
using namespace std;




struct File
{
    File(const string& path)
        : _path(path)
        , _fp(fopen(path.c_str(), "r"))
    {
        if(!_fp)
            throw runtime_error("File: failed open " + path);
    }

    ~File()
    {
        if(_fp)
        {
            fclose(_fp);
            _fp = 0;
        }
    }

    std::FILE* operator*()
    {
        return _fp;
    }

    std::FILE* _fp;
    string     _path;
};




template <class T>
struct CustomAllocator {
    typedef T value_type;

    CustomAllocator() noexcept {}

    template <class U> CustomAllocator (const CustomAllocator<U>&) noexcept
    {
    }

    T* allocate (std::size_t n)
    {
        return reinterpret_cast<T*>( ::operator new(n*sizeof(T)));
    }

    void deallocate (T* p, std::size_t n)
    {
        ::operator delete(p);
    }
};




static const string BIN_NAME = "parser";
using RecordValue_t = uint16_t;
using UpdateResultsFoo = std::function<void(unordered_map<RecordValue_t, uint64_t>&)>;




class FsIterator
{
public:
    FsIterator(const string& path,
               const std::string& extension)
        : _iterator()
        , _extension(extension)
        , _maxBufferSize(100)
        , _buffer()
    {
        _buffer.reserve(_maxBufferSize);
        auto p = filesystem::current_path().append(path);
        if (!std::filesystem::is_directory(p))
            throw runtime_error(path + " folder doesn't exists");
        _iterator = filesystem::directory_iterator(p);
    }

    std::optional<filesystem::path> next()
    {
        lock_guard<mutex> lock(_mutex);
        if(_buffer.empty())
        {
            for(size_t i=0; i<_maxBufferSize; ++i)
            {
                if(_iterator != filesystem::end(_iterator))
                {
                    if(filesystem::is_regular_file(*_iterator) &&
                       _iterator->path().extension() == _extension)
                    {
                        _buffer.emplace_back(_iterator->path());
                    }
                    ++_iterator;
                }
            }
        }

        if(!_buffer.empty())
        {
            std::optional<filesystem::path> result(move(_buffer.back()));
            _buffer.pop_back();
            return result;
        }
        else
            return {};
    }

private:
    filesystem::directory_iterator _iterator;
    std::string                    _extension;
    mutex                          _mutex;
    const size_t                   _maxBufferSize;
    vector<filesystem::path>       _buffer;
};




class Worker
{
public:
    Worker(FsIterator& iterator,
           uint32_t    bufferSize,
           UpdateResultsFoo updateResultsFoo)
        : _iterator(iterator)
        , _bufferSize(bufferSize)
        , _buffer(nullptr)
        , _thread(&Worker::foo, this)
        , _updateResultsFoo(updateResultsFoo)
    {
        _buffer = new uint8_t[_bufferSize];
    }

    ~Worker()
    {
        delete [] _buffer;
        _buffer = nullptr;
    }

    void wait()
    {
        _thread.join();
    }

private:
    void foo()
    {
        while(auto path = _iterator.next())
        {
            auto file = std::allocate_shared<File>(_fileAllocator, *path);
            size_t bytesLeft = 0;
            size_t bytesDone = 1;
            while(bytesDone)
            {
                uint8_t* bufferPtr = &_buffer[0] + bytesLeft;
                size_t bytesToRead = _bufferSize - bytesLeft;
                const auto bytesRead = fread(bufferPtr, sizeof _buffer[0], bytesToRead, file->_fp);
                bytesDone = parse(&_buffer[0], bytesRead + bytesLeft);
                bytesLeft = bytesRead + bytesLeft - bytesDone;
                memcpy(&_buffer[0], &_buffer[0] + bytesDone, bytesLeft);
                bufferPtr = &_buffer[0] + bytesLeft;
            }
        }
        flushResults();
    }

    size_t parse(uint8_t* buf, size_t size)
    {
        uint8_t* ptr = buf;
        size_t done = 0;

        while(ptr < buf + size)
        {
            uint8_t* begin = nullptr;
            uint8_t* end = nullptr;

            // Skip spaces, commas and new lines
            while(ptr < buf + size)
            {
                if(!(*ptr == ' ' || *ptr == '\n' || *ptr == ','))
                {
                    begin = ptr++; // Begin of the numeric part
                    break;
                }
                ++ptr;
            }

            // Find comma or new line delimeter
            while(ptr < buf + size)
            {
                if(*ptr == ',' || *ptr == '\n')
                {
                    end = ptr; // End of the numeric part
                    break;
                }
                ++ptr;
            }

            if(begin && end && (end > begin))
            {
                string s(begin, end);
                RecordValue_t value = stoi(s);
                ++_results[value];
            }

            if(begin && !end)
            {
                done = begin - buf;
            }
            else
            {
                done = ptr - buf;
            }
        }
        return done;
    }

    void flushResults()
    {
        _updateResultsFoo(_results);
        _results.clear();
    }

    FsIterator&           _iterator;
    uint32_t              _bufferSize;
    uint8_t*              _buffer;
    CustomAllocator<File> _fileAllocator;
    thread                _thread;
    unordered_map<RecordValue_t, uint64_t> _results;
    UpdateResultsFoo      _updateResultsFoo;
};




class Parser
{
public:
    Parser(const string& srcFolder,
           uint32_t bufferSize,
           uint16_t workersCount,
           const string& resultFile)
        : _srcFolder(srcFolder)
        , _bufferSize(bufferSize)
        , _workersCount(workersCount)
        , _resultFile(resultFile)
        , _iterator(_srcFolder, ".csv")
        , _mutex()
        , _results()
    {
    }

    void run()
    {
        const auto time_begin = std::chrono::system_clock::now();

        cout << "buffer size: " << _bufferSize << endl;
        cout << "workers count: " << _workersCount << endl;
        auto nproc = _workersCount;
        std::vector<std::unique_ptr<Worker>> workers;
        while (nproc > 0)
        {
            UpdateResultsFoo update = std::bind(&Parser::updateResults, this, placeholders::_1);
            auto worker = make_unique<Worker>(_iterator, _bufferSize, update);
            workers.emplace(workers.end(), std::move(worker));
            --nproc;
        }

        for(auto& worker : workers)
        {
            worker->wait();
        }

        // Save ordered results
        map<RecordValue_t, uint64_t> orderedResults;
        for(const auto&[value, count]: _results)
            orderedResults[value] = count;

        filesystem::path curPath = filesystem::current_path();
        curPath.append(_srcFolder);
        curPath.append(_resultFile);
        ofstream ofs(curPath.string(), ofstream::out);

        uint64_t valuesTotalCount = 0;
        for(const auto&[value, count]: orderedResults)
        {
            valuesTotalCount += count;
            ofs << value << ":" << count << ","<< endl;
        }
        ofs << "values totally:" << valuesTotalCount << endl;
        ofs.close();

        const auto time_complete = std::chrono::system_clock::now();
        auto elapsed = chrono::duration_cast<chrono::seconds>(time_complete - time_begin);
        cout << "time elapsed(seconds):" << elapsed.count() << endl;
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

    string     _srcFolder;
    uint32_t   _bufferSize;
    uint16_t   _workersCount;
    string     _resultFile;
    FsIterator _iterator;
    mutex      _mutex;
    unordered_map<RecordValue_t, uint64_t> _results;

};




void print_help(const options_description& od)
{
    cout << "Parse a bunch of files containing test data" << std::endl
         << "example: ./" << BIN_NAME 
         << " --src-folder=output --buffer-size=4096 --workers-count=4 --result-file=result-4096-4.index"
         << std::endl << od;
}




int main(int argc, char* argv[])
{
    unique_ptr<Parser> parser;
    options_description general("Options");

    try
    {
        general.add_options()
            ("help", "produce a help message")
            ("src-folder", value<string>(), "source folder with files to parse")
            ("buffer-size", value<uint32_t>(), "buffer size in bytes")
            ("workers-count", value<uint16_t>(), "amount of workers to create")
            ("result-file", value<string>(), "file to strore parsing results to, i.e <value>:<count> pairs")
            ;

        variables_map vm;
        store(parse_command_line(argc, argv, general), vm);

        if (vm.count("help")) 
        {
            print_help(general);
            return 0;
		}

        string srcFolder;
        uint32_t bufferSize;
        uint16_t workersCount;
        string resultFile;

        if (!vm.count("src-folder"))
            throw std::invalid_argument("missing src-folder");
        srcFolder = vm["src-folder"].as<string>();

        if (!vm.count("workers-count"))
            throw std::invalid_argument("missing workers-count");
        workersCount = vm["workers-count"].as<uint16_t>();

        if (!vm.count("buffer-size"))
            throw std::invalid_argument("missing buffer-size");
        bufferSize = vm["buffer-size"].as<uint32_t>();

        if (!vm.count("result-file"))
            throw std::invalid_argument("missing result-file");
        resultFile = vm["result-file"].as<string>();

        cout << "src-folder:" << srcFolder << endl;
        cout << "result-file:" << resultFile << endl;

        parser = make_unique<Parser>(srcFolder, bufferSize, workersCount, resultFile);
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

    parser->run();

    return 0;
}
