from single_header import extract_header_name


def test_extract_header_name1():
    assert extract_header_name('#include<aaa>') == '<aaa>'
    assert extract_header_name('  #include  <aaa>  ') == '<aaa>'
    assert extract_header_name('  #  include <aaa/bbb/ccc.h>  // comment  ') == '<aaa/bbb/ccc.h>'
    assert extract_header_name(r'  #include <aaa\bbb\ccc.h>  // comment  ') == r'<aaa\bbb\ccc.h>'
    assert extract_header_name(r'  #  include <aaa/bbb\ccc.h>  // comment  ') == r'<aaa/bbb\ccc.h>'


def test_extract_header_name2():
    assert extract_header_name('#include"aaa"') == '"aaa"'
    assert extract_header_name('  #include  "aaa"  ') == '"aaa"'
    assert extract_header_name('  #  include "aaa/bbb/ccc.h"  // comment  ') == '"aaa/bbb/ccc.h"'
    assert extract_header_name(r'  #include "aaa\bbb\ccc.h"  // comment  ') == r'"aaa\bbb\ccc.h"'
    assert extract_header_name(r'  #  include "aaa/bbb\ccc.h"  // comment  ') == r'"aaa/bbb\ccc.h"'


def test_extract_header_name3():
    assert extract_header_name('') is None
    assert extract_header_name('   ') is None
    assert extract_header_name('\n') is None
    assert extract_header_name('    \n') is None
    assert extract_header_name('  abcd   ') is None
    assert extract_header_name('#pragma once\n') is None
