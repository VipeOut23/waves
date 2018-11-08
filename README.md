Waves.	 A little toy for plotting a sinus curve with text (stdin)

```text
Usage:
  waves [-t <text>] [-w <num>] [-W <num>] [-s <num>] [...]

Options:
  -t --text		 Specify text to use in the curve instead of stdin
  -w --width		 Specify the width of a text "chunk"(default: 1)
  -W --output-width	 Specify the amplitude of the curve
			 (e.g. Terminal width) (default: 80
  -s --steps		 How many steps are made per curve cycle
			 (somewhat the reverse frequency) (default: 20)
  --sin-precision	 Specify, how many iterations of the taylor
			 sequence are used; Sane: 8-11(Default: 9)
  --sources		 Save the sources in ./waves-src.tgz
  -? -h --help		 Well, you're here now
```
