
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <functional>
// WARNING: NO more headers allowed!

using std::pair;
using std::string;
using std::vector;
using std::map;
using std::function;
namespace final {
    class arguments {
    private:
        // WARNING: You cannot add more member variables.
        int _argc;
        char** _argv;
    public:
        arguments() : _argc(0), _argv(nullptr) {}
        explicit arguments(const string& cmd) {
            // todo implement constructor
            _argc = 0;
            _argv = nullptr;
            
            if (cmd.empty()) {
                return;
            }
            
            // Count the number of arguments by counting spaces
            // We'll use a vector to store the start positions and lengths of each argument
            vector<pair<size_t, size_t>> args;
            size_t start = 0;
            size_t len = cmd.length();
            bool in_arg = false;
            
            for (size_t i = 0; i <= len; i++) {
                if (i < len && cmd[i] != ' ') {
                    if (!in_arg) {
                        start = i;
                        in_arg = true;
                    }
                } else {
                    if (in_arg) {
                        args.push_back({start, i - start});
                        in_arg = false;
                    }
                }
            }
            
            _argc = args.size();
            if (_argc == 0) {
                return;
            }
            
            // Allocate memory for argv array
            _argv = new char*[_argc];
            
            // Allocate memory for each string and copy the content
            for (int i = 0; i < _argc; i++) {
                size_t arg_len = args[i].second;
                _argv[i] = new char[arg_len + 1];
                std::strncpy(_argv[i], cmd.c_str() + args[i].first, arg_len);
                _argv[i][arg_len] = '\0';
            }
        }
        ~arguments() {
            // todo implement destructor
            if (_argv != nullptr) {
                for (int i = 0; i < _argc; i++) {
                    delete[] _argv[i];
                }
                delete[] _argv;
                _argv = nullptr;
            }
            _argc = 0;
        }
        // WARNING: You cannot modify the following functions
        int argc() const { return _argc; }
        char** argv() const { return _argv; }
    };

    // You don't need to modify shell.
    class shell {
    private:
        map<int, arguments> running_list;
    public:
        shell() = default;

        void run(int pid, const string& cmd, const function<void(int, char**)>& invoked) {
            running_list.emplace(pid, cmd);
            invoked(running_list[pid].argc(), running_list[pid].argv());
        }

        int subprocessExit(int pid, int return_value) {
            running_list.erase(pid);
            return return_value;
        }
        vector<int> getRunningList() const {
            vector<int> rt;
            for (auto& pair:running_list)rt.push_back(pair.first);
            return rt;
        }
    };
}
