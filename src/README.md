####################################
#           Paper Trader           #
####################################

<Prerequisites>

[0. JSON-C] - https://github.com/json-c/json-c

$ git clone https://github.com/json-c/json-c.git
$ mkdir json-c-build
$ cd json-c-build
$ cmake ../json-c   # See CMake section below for custom arguments

[1. Setting]
$ ./setup.sh

[2. Makefile]
$ make all

[3. Exexution]
$ ./menu


<Description>

data/CSV/  - JSON -> CSV로 변환했을 때 저장되는 장소
data/JSON/ - Tradier API로 통해 JSON 파일 저장되는 장소
list/      - ticker에 대한 list 파일 저장되어 있는 장소
