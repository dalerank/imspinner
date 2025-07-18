# Generate C sources from "imspinner.h" written in C++.
#
#     Input file: ./imspinner.h
#     Output files:
#       ./cimspinner.h
#       ./cimspinner.cpp
#     (Config file: ./cimspinner_config.h (written by hand))
#
#     Usage:
#       $ ruby genCimSpinner.rb

Str1 = <<EOF
/*
 * The MIT License (MIT)

 * Copyright (c) 2025 dinau

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
EOF

ImSpinnerHeader = "imspinner.h"
OutCpp          = "cimspinner.cpp"
OutHeader       = "cimspinner.h"

File.open(ImSpinnerHeader){|fp|
  puts "Reading .. #{ImSpinnerHeader}\n\n"
  cppFile = []
  cppFile.push "#include \"cimspinner_config.h\""
  cppFile.push "\n#include \"imspinner.h\""
  cppFile.push "#include \"cimspinner.h\""
  cppFile.push ""
  cppFile.push Str1
  cfuncHeader = []
  cfuncHeader.push "\n#pragma once"
  cfuncHeader.push "#ifdef __cplusplus"
  cfuncHeader.push "extern \"C\" {"
  cfuncHeader.push "#endif"
  cfuncHeader.push "\n#ifndef CIMSPINNER_API\n  #define CIMSPINNER_API\n#endif"
  cfuncHeader.push ""
  aryEnableDefine = []
  fp.each {|line|
    if  line =~ /\s+inline\s+void\s+(Spinner\w+)\((.+)\)/
      cppArgsLong = []
      cppArgsShort = []
      cArgsLongDef = []
      cArgsShortDef = []
      #
      funcname = $1
      cppArgs = $2
      ######
      if line =~/LeafColor/
        cfuncHeader.push "typedef ImColor LeafColor(int);"
      end
      ######
      cppFile.push  "\n/* C++ definition"
      cppFile.push  "   void Spinner::#{funcname}(#{cppArgs.gsub("/*","").gsub("*/","")})"
      cppFile.push  "*/"
      # get short argumants
      # void SpinnerFadeBars(const char *label, float w, const ImColor &color = white, float speed = 2.8f, size_t bars = 3, bool scale = false)
      #  cppArgs -->         const char *label, float w, const ImColor &color = white, float speed = 2.8f, size_t bars = 3, bool scale = false
      cppArgs.split(",").each {|arg|
        if not arg.include? "="
          # C function args short
          cArgsShortDef.push arg.gsub("&","").gsub("/*","").gsub("*/","")
          # Cpp args short
          cppArgsShort.push arg.gsub("/*","").gsub("*/","").split(" ").last.gsub("*","").gsub("&","")
        else
          cArgsLongDef.push arg.split("=")[0].gsub("&","").strip
          cppArgsLong.push  arg.split("=")[0].split(" ").last.gsub("&","")
        end
      }
      #-----------------------------------------------------------
      # Generate C functions in header: Short arguments functions
      #-----------------------------------------------------------
      sEnableDefine = "SPINNER_#{funcname.split("Spinner")[1].upcase}"
      aryEnableDefine.push  "//#define #{sEnableDefine}"
      cargs = cArgsShortDef.join(",")
      cfunc = "CIMSPINNER_API void #{funcname}(#{cargs})"
      cfuncHeader.push  cfunc + ";\n"
      #--------------------------------------------------
      # Generate cpp functions that have short arguments
      #--------------------------------------------------
      cppFile.push "#ifdef #{sEnableDefine}"
      cppFile.push "#{cfunc} \{"
      shortArgs = cppArgsShort.join(", ")
      cppFile.push "  ImSpinner::#{funcname}(#{shortArgs});"
      cppFile.push "}"
      #-----------------------------------------------------------
      # Generate C functions in header : Long arguments functions
      #-----------------------------------------------------------
      if 0 != cArgsLongDef.length
        cargs = cargs + ","  + cArgsLongDef.join(", ")
        cfunc = "CIMSPINNER_API void #{funcname}Ex(#{cargs})"
        cfuncHeader.push  cfunc + ";\n"
        cppFile.push "#{cfunc} \{"
        ##-------------------------------------------------
        ## Generate cpp functions that have long arguments
        ##-------------------------------------------------
        longArgs = cppArgsLong.join(", ")
        cppFile.push "  ImSpinner::#{funcname}(#{shortArgs}, #{longArgs});"
        cppFile.push "}"
      end
      cppFile.push "#endif"
    end
  }

  MsgAutogen = "/*\n * This file has been auto generated by srcript, see genCimSpinner.rb\n */\n"
  # Output CPP source
  File.open(OutCpp,"w") {|fp|
    puts "Generated: #{OutCpp}"
    fp.puts MsgAutogen
    cppFile.each {|line|
      fp.puts line
    }
    fp.puts "\n#ifdef IMSPINNER_DEMO"
    fp.puts "CIMSPINNER_API void demoSpinners() {\n  ImSpinner::demoSpinners(); \n}"
    fp.puts "#endif"
  }

  # Output C Header file
  File.open(OutHeader,"w") {|fp|
    puts "Generated: #{OutHeader}"
    fp.puts MsgAutogen
    fp.puts "#include \"cimspinner_config.h\""
    aryEnableDefine.each {|line|
      fp.puts line.chomp
    }
    cfuncHeader.each {|line|
      fp.puts line.chomp
    }
    fp.puts "\nCIMSPINNER_API void demoSpinners();"
    fp.puts "#ifdef __cplusplus"
    fp.puts "}"
    fp.puts "#endif"
  }
}
puts "\nEnd genaration"
