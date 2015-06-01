# annoycode

Annoy people in a friendly fashion by converting Unicode characters to other
characters that looks the same. It could be symbols like 'V' and the Roman
literal five 'V'. Imagine a path with unicode characters where an "å" is not
U+00E5 but an "a" (U+0061) with a circle over it (U+030A). That of course means
that the path doesn't exist even though it looks exactly like the original one.

The project was born due to a similar path/Unicode bug at the office.

# Usage

The main program _annoycode.py_ is used in the following way:
```
usage: annoycode.py <input file> <output file>
Substitutes symbols with ones that look similar.
```

Since the idea is to find symbols that look similar, then I have also
implemented _inspect.py_ to be able to visualize the substituted results:

```
usage: inspect.py <input file>
Writes contents of file as UTF-8 encoded content to STDOUT.
```

Also, the data file with substitutions is pickled in a binary format, so you can inspect the values like this:
```
usage: ./inspectdata.py <data file>
Inspects a substitutions data file.
```

# Examples

```
% ./annoycode.py test.txt test.out.txt
4 substitutions, 5 -> 9 bytes, +80.0%

% ./inspect.py test.txt
=== 5 raw bytes ===
b'ABC!\n'

=== 5 rendered symbols ===
ABC!

% ./inspect.py test.out.txt
=== 9 raw bytes ===
b'\xce\x91\xce\x92\xd0\xa1\xc7\x83\n'

=== 5 rendered symbols ===
ΑΒСǃ

```

You can also go back again:
```
% ./annoycode.py test.out.txt test2.out.txt
4 substitutions, 9 -> 5 bytes, -44.44444444444444%

% ./inspect.py test2.out.txt
=== 5 raw bytes ===
b'ABC!\n'

=== 5 rendered symbols ===
ABC!

```

# Dependencies

It is required to have the following installed:
- Python 3+
- Pillow 2.8+

# Training data

Data must be in hand in order to be able to substitute symbols and that is done
using the _trainer.py_. I have already included a file with some substitutions
in _data.p_ that will be implicitly used by _annoycode.py_.
