import os

MAX_SEARCH_RESULTS = 10

def beam_search(filename, current_path, beam):
    for root, dirs, files in os.walk(current_path):
        for name in files + dirs:
            if name == filename:
                result_path = os.path.join(root, name)
                beam.append(result_path)
                if len(beam) >= MAX_SEARCH_RESULTS:
                    return
        if len(beam) >= MAX_SEARCH_RESULTS:
            return

def search_files(filename):
    beam = []
    drives = ['A:', 'B:', 'C:', 'D:', 'E:', 'F:', 'G:', 'H:', 'I:', 'J:', 'K:', 'L:', 'M:', 'N:', 'O:', 'P:', 'Q:', 'R:', 'S:', 'T:', 'U:', 'V:', 'W:', 'X:', 'Y:', 'Z:']

    for drive in drives:
        beam_search(filename, drive + '\\', beam)
    
    return beam

def main():
    filename = input("Enter the file or folder name to search: ")
    search_results = search_files(filename)

    if len(search_results) > 0:
        print("Search Results:")
        for i, path in enumerate(search_results, 1):
            print(f"{i}. {path}")
    else:
        print("No results found.")

if __name__ == '__main__':
    main()
