// -*- mode: c++; indent-tabs-mode: nil; -*-
//
// Copyright (c) 2009-2013 Illumina, Inc.
//
// This software is provided under the terms and conditions of the
// Illumina Open Source Software License 1.
//
// You should have received a copy of the Illumina Open Source
// Software License 1 along with this program. If not, see
// <https://github.com/downloads/sequencing/licenses/>.
//

/// \file
///
/// an efficient (and slightly unsafe) class for basic tab-delimited files, etc...
///

/// \author Chris Saunders
///

#include "blt_util/blt_exception.hh"
#include "blt_util/istream_line_splitter.hh"

#include <cassert>
#include <cstring>

#include <iostream>
#include <sstream>



void
istream_line_splitter::
dump(std::ostream& os) const {
    os << "\tline_no: " << _line_no << "\n";
    os << "\tline: '";
    for(unsigned i(0); i<_n_word; ++i) {
        if(i) os << _sep;
        os << word[i];
    }
    os << "'\n";
}



bool
istream_line_splitter::
parse_line() {
    _n_word=0;
    _line_no++;
    _is.getline(_buf,_buf_size);
    const unsigned len(strlen(_buf));
    if((len+1) >= _buf_size) {
        std::ostringstream oss;
        oss << "ERROR: input exceeds buffer size on line_no: " << _line_no << "\n\n";
        throw blt_exception(oss.str().c_str());
    }

    if(! _is) {
        if(_is.eof()) { return false; } // normal eof:

        std::ostringstream oss;
        oss << "ERROR: Unexpected read failure in parse_line().\n";
        throw blt_exception(oss.str().c_str());
    }

    if(NULL == _buf) return false;
    assert(len);

    // do a low-level separator parse:
    {
        char* p(_buf);
        word[0]=p;
        unsigned i(1);
        while(i<_max_word) {
            if((*p == '\n') || (*p == '\0')) break;
            if (*p == _sep) {
                *p = '\0';
                word[i++] = p+1;
            }
            ++p;
        }
        _n_word=i;
    }
    return true;
}
