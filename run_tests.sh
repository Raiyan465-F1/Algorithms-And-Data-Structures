
find_cmake_subdirs() {

    start_dir="$1"
 
    # Find all subdirectories with depth 1.
    subdirs=$(find "$start_dir" -maxdepth 1 -type d)

    # Call the function recursively for each subdirectory that does not contain a CMakeLists.txt file.
    for subdir in $subdirs; do
        if [ ! -f "$subdir/CMakeLists.txt" ]; then
            if [ "$subdir" != "$start_dir" ]; then
                find_cmake_subdirs "$subdir"
            fi
        else
            echo "$subdir"
        fi
    done
}

test_cpp_projects() {
    subdirs=$(find_cmake_subdirs .)
    current_dir=$(pwd)

    # Run tests for each subdirectory.
    for subdir in $subdirs; do
        echo "Running tests for $subdir"
        cd "$subdir"
        mkdir -p build && cd build
        cmake .. && make
        ctest --verbose
        cd .. && rm -rf build
        cd "$current_dir"
    done
}

find_python_subdirs() {

    start_dir="$1"
 
    # Find all subdirectories with depth 1.
    subdirs=$(find "$start_dir" -maxdepth 1 -type d)

    # Call the function recursively for each subdirectory that does not contain a CMakeLists.txt file.
    for subdir in $subdirs; do
        if [ ! -f "$subdir/__init__.py" ]; then
            if [ "$subdir" != "$start_dir" ]; then
                find_python_subdirs "$subdir"
            fi
        else
            echo "$subdir"
        fi
    done
}

test_python_projects() {

    subdirs=$(find_python_subdirs .)
    current_dir=$(pwd)

    # Run tests for each subdirectory.
    for subdir in $subdirs; do
        echo "Running tests for $subdir"
        cd "$subdir"
        python3 -m unittest discover -v
        cd "$current_dir"
    done    
}

main() {

    if [ "$#" -eq 0 ]; then
        echo "Running tests for all projects"

        echo "Running tests for Python projects"
        test_python_projects

        echo "Running tests for C++ projects"
        test_cpp_projects
    fi

    if [ "$#" -eq 1 ]; then
        if [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
            echo "Usage: run_tests.sh [OPTION]"
            echo "Run tests for all projects."
            echo ""
            echo "Options:"
            echo "  -h, --help    Show this help message and exit"
            echo "  -p, --python  Run tests for Python projects"
            echo "  -c, --cpp     Run tests for C++ projects"
            exit 0
        fi
        if [ "$1" == "-p" ] || [ "$1" == "--python" ]; then
            echo "Running tests for Python projects"
            test_python_projects
            exit 0
        fi
        if [ "$1" == "-c" ] || [ "$1" == "--cpp" ]; then
            echo "Running tests for C++ projects"
            test_cpp_projects
            exit 0
        fi
    fi

    if [ "$#" -gt 1 ]; then
        echo "Too many arguments"
        exit 1
    fi

}

main "$@"