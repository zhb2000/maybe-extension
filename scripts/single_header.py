import re
import os
import os.path
import glob
import pathlib
from typing import Iterable, Optional, List, Dict


def collect_files(proj_root: str) -> List[str]:
    return glob.glob(os.path.join(proj_root, '**/*.hpp'), recursive=True)


def extract_header_name(line: str) -> Optional[str]:
    m = re.match(r'\s*#\s*include\s*(<[^>]+>)', line)
    if m is not None:
        return m[1]
    m = re.match(r'\s*#\s*include\s*("[^"]+")', line)
    if m is not None:
        return m[1]
    return None


def find_header(header: str, cur_dir: str, proj_root: str) -> Optional[str]:
    filename = header[1:-1]
    if header[0] == '"' and header[-1] == '"':
        filepath = os.path.join(cur_dir, filename)  # find from current directory
        if os.path.isfile(filepath):
            return filepath
        filepath = os.path.join(proj_root, filename)  # find from project include root
        if os.path.isfile(filepath):
            return filepath
        return None
    else:
        assert header[0] == '<' and header[-1] == '>'
        filepath = os.path.join(proj_root, filename)  # find from project include root
        if os.path.isfile(filepath):
            return filepath
        return None


def extract_dependencies(filename: str, proj_root: str, encoding='utf8') -> List[str]:
    dependencies = []
    cur_dir = os.path.dirname(filename)
    with open(filename, encoding=encoding) as file:
        for line in file:
            header = extract_header_name(line)
            if header is None:  # line is not an `#include` line
                continue
            header_path = find_header(header, cur_dir, proj_root)
            if header_path is None:  # header is not a project file
                continue
            dependencies.append(header_path)
    return dependencies


def build_graph(files: Iterable[str], proj_root: str, encoding='utf8') -> Dict[str, List[str]]:
    g = {filename: extract_dependencies(filename, proj_root, encoding)
         for filename in files}

    def format_path(path: str) -> str:
        return str(pathlib.PurePosixPath(pathlib.Path(path)))

    graph = {format_path(u): [format_path(v) for v in g[u]]
             for u in g}
    return graph


def topo_sort(graph: Dict[str, List[str]]) -> List[str]:
    g = {u: [] for u in graph}
    for u, deps in graph.items():
        for v in deps:
            g[v].append(u)
    vis = {u: 0 for u in g}
    topo = []

    def dfs(u: str) -> bool:
        vis[u] = -1
        for v in g[u]:
            if vis[v] == -1:
                return False
            elif vis[v] == 0:
                if not dfs(v):
                    return False
        vis[u] = 1
        topo.append(u)
        return True

    for u in g:
        if vis[u] == 0:
            if not dfs(u):
                raise Exception('cannot topo sort')
    topo.reverse()
    return topo


def filtered_lines(filepath: str, proj_root: str, encoding='uft8'):
    cur_dir = os.path.dirname(filepath)

    def is_include_project_file(line: str) -> bool:
        header = extract_header_name(line)
        if header is None:  # not an `#include` line
            return False
        # check if the included header is project file
        return find_header(header, cur_dir, proj_root) is not None

    def is_pragma_once(line: str) -> bool:
        return re.match(r'\s*#\s*pragma\s*once', line) is not None

    with open(filepath, encoding=encoding) as f:
        for line in f:
            if not is_include_project_file(line) and not is_pragma_once(line):
                yield line.replace('\n', '').replace('\r', '')
