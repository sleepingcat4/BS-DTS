import os
import time

MAX_SEARCH_RESULTS = 10
BEAM_WIDTH = 3

class SearchNode:
    def __init__(self, path, parent=None):
        self.path = path
        self.parent = parent

def beam_search(filename):
    beam = [SearchNode(drive + '\\') for drive in os.listdir('/') if os.path.isdir(drive + '\\')]
    results = []

    while beam and len(results) < MAX_SEARCH_RESULTS:
        next_beam = []
        for node in beam:
            for name in os.listdir(node.path):
                new_path = os.path.join(node.path, name)
                if os.path.isfile(new_path) and name == filename:
                    results.append(new_path)
                    if len(results) >= MAX_SEARCH_RESULTS:
                        return results
                elif os.path.isdir(new_path):
                    next_beam.append(SearchNode(new_path, node))
        beam = sorted(next_beam, key=lambda x: os.path.getmtime(x.path))[:BEAM_WIDTH]
    
    return results

def main():
    filename = input("Enter the file or folder name to search: ")

    start_time = time.time()
    search_results = beam_search(filename)
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
