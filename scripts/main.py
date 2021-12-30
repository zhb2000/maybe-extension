import os
import os.path
from typing import Dict, List, Iterable
from single_header import collect_files, build_graph, topo_sort, filtered_lines


def main():
    proj_root = '../include'
    assert os.path.isdir(proj_root)
    files = [f for f in collect_files(proj_root)
             if os.path.basename(f) != 'maybe_ext.hpp']
    print('Project Headers:')
    for x in files:
        print(x)

    graph = build_graph(files, proj_root, encoding='utf8')
    print('\nDependencies Graph:')
    print_graph(graph)

    sorted_files = topo_sort(graph)
    print('\nSorted Headers:')
    for x in sorted_files:
        print(os.path.basename(x))

    out_path = '../single_include/maybe_ext/maybe_ext.hpp'
    merge_headers(sorted_files, proj_root, out_path)


def print_graph(graph: Dict[str, List[str]]):
    for key, deps in graph.items():
        print(f'[{key}]')
        for dep in deps:
            print(f'- {dep}')


def merge_headers(sorted_files: Iterable[str], proj_root: str, out_path: str):
    with open(out_path, mode='w', encoding='utf8') as out_file:
        out_file.write('#ifndef MAYBE_EXT_HPP\n' +
                       '#define MAYBE_EXT_HPP\n\n')  # begin single
        for header_path in sorted_files:
            out_file.write(f'// region {os.path.basename(header_path)}\n\n')  # begin header
            for line in filtered_lines(header_path, proj_root, 'utf8'):
                out_file.write(line)
                out_file.write('\n')
            out_file.write('\n// endregion\n\n')  # end header
        out_file.write('#endif\n')  # end single


if __name__ == '__main__':
    main()
