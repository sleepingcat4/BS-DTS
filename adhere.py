import os
import time

MAX_SEARCH_RESULTS = 10

def dfs_search(filename, current_path, results):
    for root, dirs, files in os.walk(current_path):
        for name in files + dirs:
            new_path = os.path.join(root, name)
            if name == filename:
                results.append(new_path)
                if len(results) >= MAX_SEARCH_RESULTS:
                    return

def search_files(filename):
    results = []
    drives = ['A:', 'B:', 'C:', 'D:', 'E:', 'F:', 'G:', 'H:', 'I:', 'J:', 'K:', 'L:', 'M:', 'N:', 'O:', 'P:', 'Q:', 'R:', 'S:', 'T:', 'U:', 'V:', 'W:', 'X:', 'Y:', 'Z:']

    for drive in drives:
        dfs_search(filename, drive + '\\', results)
    
    return results

def main():
    filename = input("Enter the file or folder name to search: ")

    start_time = time.time()
    search_results = search_files(filename)
    end_time = time.time()

    if search_results:
        print("Search Results:")
        for i, path in enumerate(search_results, 1):
            print(f"{i}. {path}")
    else:
        print("No results found.")

    execution_time = end_time - start_time
    print(f"Execution Time: {execution_time:.4f} seconds")

if __name__ == '__main__':
    main()
