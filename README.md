# annoycode

Annoy people in a friendly fashion by converting Unicode characters to other
characters that looks the same. It could be symbols like 'V' and the Roman
literal five 'V'. Imagine a path with unicode characters where an "å" is not
U+00E5 but an "a" (U+0061) with a circle over it (U+030A). That of course means
that the path doesn't exist even though it looks exactly like the original one.

The project was born due to a similar path/Unicode bug at the office.

# Usage

The main program _annoycode_ is used in the following way:
```
Usage: annoycode [options] <input file> <output file>
Substitues Unicode symbols that look similar.

Options:
  -h, --help              Displays this help.
  -f, --file <data file>  Use substitutions from <data file>. Will use
                          "data.json" otherwise.

Arguments:
  input file              File to read values from.
  output file             File to write results to.
```

Since the idea is to find symbols that look similar, then I have also
implemented _inspect_ to be able to visualize the substituted results:

```
Usage: inspect [options] <input file>
Inspects Unicode file contents.

Options:
  -h, --help  Displays this help.

Arguments:
  input file  File to read values from.
```

# Examples

```
% ./annoycode test.txt test.out.txt
Loaded 23919 substitutions from data.json
3 substitutions applied, 5 -> 8 bytes, +60%

% ./inspect test.txt
=== 5 raw bytes ===
ABC!

=== 5 rendered symbols ===
ABC!

% ./inspect test.out.txt
=== 8 raw bytes ===
\u913\u914\u1057!

=== 5 rendered symbols ===
ΑΒС!

```

# Dependencies

It is required to have the following installed:
- C++11 compliant compiler: Clang or GCC preferebly
- Cmake 3+
- Qt 5+ framework

# Training data

Data must be in hand in order to be able to substitute symbols and that is done
using the _trainer_. I have already included a file with some substitutions
in _data.json_ that will be implicitly used by _annoycode_ and _inspect_ if run
in the same folder.

```
Usage: trainer [options]
Trains data by finding pairs of Unicode symbols that renders to similar text.

Options:
  -h, --help              Displays this help.
  -f, --file <data file>  Use substitutions from <data file>. Will use
                          "data.json" otherwise.
  -t, --threads <num>     Use <num> of threads to search for mathes. Defaults
                          to using all available ones, with value "0".
  -i, --inc-save          Incrementally save results to disk.
```
