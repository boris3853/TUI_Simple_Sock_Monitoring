# TUI_Simple_Stock_Monitoring

## 프로그램 명
Simple Stock Monitoring Program based on TUI(Text User Interface)  

## 프로그램 개요
Tradier사 주식 API를 통해 현재 미국 주식 시장을 모니터링하고, 이전의 미국 주식 시장 데이터를 통해 주식 시장에 대한 공부나 투자 전략을 세울 수 있도록 
시뮬레이션 하는 프로그램이다.  

## 프로그램 기능
###	1. 기본 메뉴
1. 크롤링할 미국시장에 대한 (날짜, 시간 간격) 정보를 기입
2. 현재 미국시장을 모니터링
3. 종료 중 1가지 메뉴를 선택할 수 있도록 설정할 수 있도록 한다.

![image](https://user-images.githubusercontent.com/60434800/166079434-ee2b0e55-f2d0-4820-9c34-81cbb877c280.png)

Timer Signal을 통해 10초 이내에 선택하지 않으면 자동으로 선택한다.

```	Tradier Sandbox API : https://documentation.tradier.com/brokerage-api/markets/get-quotes ```


### 2. 메뉴 1
미국 시장 데이터 기입: 날짜 데이터와 시간 간격 데이터를 입력 받는다.

날짜 데이터는 현재 날짜로부터 1min 간격일 때 10일 전 / 5min 간격일 때 18일 전 / 15min 간격일 때 18일 전 데이터를 조회할 수 있다.
![image](https://user-images.githubusercontent.com/60434800/166079570-f135a678-8da1-4d7d-a3d1-2eb47ab9911f.png)
![image](https://user-images.githubusercontent.com/60434800/166079587-2099e9f8-83ae-48a3-a7c0-f4a374d093a2.png)

시간 간격 데이터는 1min / 5min / 15min 간격을 입력 받은 후 데이터를 조회할 수 있다.

### 3.	메뉴 2
시장 모니터링 : 유명한 나스닥 상장코드 리스트 중에 한 종목을 선택할 수 있도록 커서를 옮길 수 있도록 하고 해당 종목에 대한 차트 그래프 혹은 데이터 리스트를 나열한다.
![image](https://user-images.githubusercontent.com/60434800/166079604-0ebd3d1d-da7c-4437-b168-679eb57457e2.png)

다음과 같이 JSON 형식으로 종목 데이터를 받아 파싱 한 이후 리스트를 나열한다.
![image](https://user-images.githubusercontent.com/60434800/166079615-289f4dd5-abce-4ee3-b660-b20216172c06.png)
![image](https://user-images.githubusercontent.com/60434800/166079618-f52bfbe9-dafa-4361-8a75-615f21e140cb.png)


### 4.	메뉴 3
EXIT: 선택 시 프로그램 종료한다.

## 상세 구조
### 1.	기본 메뉴
 ![image](https://user-images.githubusercontent.com/60434800/166079637-de52dfac-b867-4196-95d2-ad5f7f31f370.png)

기본 메뉴는 Main_menu로 바인딩된 Main_menu_win로 구성되어 있다.

window 크기는 화면 크기에 맞춰서 조정이 된다.

프로그램 실행 시 Timer 시그널을 등록하여 매 초마다 시그널을 주며 left_time 변수가 양수가 아니면 자동으로 메뉴를 선택 되도록 설정되어 있다. (SELECTED 표시 후 아무 키 입력 한번 해야됨)

이 때 타이머 구성은 fork()를 통해서 Child process를 생성하여 Timer에 대한 SIG_ALRM을 받고 Parent process는 메인 루틴을 따른다.

메인 루틴의 경우, main_menu_win 로부터 키보드 입력을 받아서 위 / 아래 / Enter(10) 값을 처리한다.

Enter 키를 받았을 때, 해당 Item이 Menu1이면 form() [form.c 참고] 함수를 Menu2이면 monitor.c [menu2.c 참고] 3번 Menu이면 while문 밖으로 나가 종료하게 된다.

이때 switch - case문을 종료하기 전에 erase()와 (WIN *, Menu *)를 통해 다른 메뉴에서 메인 메뉴로 넘어왔을 때 정상적으로 메뉴를 출력할 수 있도록 코드를 추가해주었다.

그리고 Enter키를 입력 받았을 때 처음에 SIG_ALRM을 잠시 무시하고 다른 메뉴들을 처리한 이후에 break 되기 전에 SIG_ALRM을 재등록하고, timer도 다시 설정해준다.

프로그램이 종료되기 전에 Main_menu와 Main_menu 안의 total_item 에 대한 동적 메모리를 해제한다.

### 2.	메뉴 1 (미국 시장 데이터 기입)
![image](https://user-images.githubusercontent.com/60434800/166079741-9824552e-90e4-4476-8555-3f5d581fc88b.png)

메뉴 1은 field로 바인딩된 form으로 구성되어 있다.

from의 경우 데이터를 기입할 수 있는 위치가 고정되어 있다.

프로그램 실행 시 JSON 파일을 생성했다는 SIGUSR1 시그널을 받는 (sigaction) json_sa을 등록한다.

form 은 년도/월/일/시간 간격 정보를 f_y / f_m / f_d / f_ITV Element를 받으며, 해당 필드 Element를 받을 때는 Integer Type으로 받을 수 있는 데이터 범위가 설정되어 있다.

form의 모든 데이터 값들을 다 기입한 이후에 Enter을 누르게 되면 JSON FILE을 받는다는 표시를 해주면서 get_json(int, int, int, int) [ get_Delay_Data.c 참고] 함수를 실행시켜주는 데, Form에서  받은 Y / M / D / interval 데이터를 통해 curl 명령어를 조합하여 JSON 데이터를 받고, 이를 data/JSON/AAPL.json 형식으로 File Pointer을 통해 .json 파일을 저장한다.

이때 json 파일을 다 생성한 경우에 FP를 해제하고, fork()를 통해 Child Process가 SIGUSR1 시그널을 주도록 Kill 명령어를 사용한 후 종료 시킨다. (메인 루틴에 통신 가능)
다시 메인 루틴으로 넘어와 SIGUSR1 시그널을 받을 때마다 진행 사항을 표기하고 json 파일을 다 받을 시에 다 받았다고 표시한다.

프로그램이 종료되기 전에 form 와 field 에 대한 동적 메모리를 해제한다.

### 3. 메뉴 2 (미국 시장 모니터링)
![image](https://user-images.githubusercontent.com/60434800/166079756-2728e20f-d74c-49a9-9407-edcb2b653f15.png)

메뉴 2는 ticker_menu로 바인딩된 ticker_menu_win과 table_menu로 바인딩 된 table_menu_win으로 구성되어 있다.

ticker_menu_win과 table_menu_win 크기는 화면 크기에 맞춰서 조정이 된다.

프로그램 실행 시 menu와 window에 대한 설정을 해준 후 int json_to_csv() [ JSON_TO_CSV.c 참고] 를 실행시켜 몇 개의 데이터를 조회해야 하는 지 length 값을 받아 n_table에 저장한다.

json_to_csv() 의 경우 get_list() [ get_list().c 참고, 앞서 언급한 get_Delay_Data.c도 get_list() 사용함]를 통해 받아올 Ticker(종목)들을 불러와 변환할 URI(ex. data/JSON/AAPL.json)와 저장할 URI(ex. data/CSV/AAPL.csv)를 설정한다.

json-c를 통해 series – data를 조회한 이후 time / open / high / low /close / volume 데이터를 파싱하여 이를 csv 형태로 변환 후 저장한다. 이후 몇 개의 데이터를 조회해야 하는 지 값을 반환한다.

Ticker 메뉴에서 특정 Ticker을 위-아래 방향키를 통해 선정할 수 있으며, 오른쪽 방향키를 입력하면 입력 값을 받는 대상이 ticker_menu_win에서 table_menu_win으로 변경되며 해당 Ticker에 대한 데이터를 나열한다.

CSV 파일로부터 데이터를 받아와 table_item을 재구성 하여 table_menu를 업데이트하며, table_menu_win 쪽으로  입력으로 받게 된다.

아까와 같이 위-아래 방향키를 통해 데이터를 조회할 수 있으며 왼쪽 방향키를 입력하게 되면 입력 값을 받는 대상이 table_menu_win에서 ticker_menu_win으로 변경된다.

이후 F1 키를 입력 받으면 monitor 함수가 종료가 되며 menu 와 item에 대한 동적 메모리를 해제한다.

### 4.	Shell script (MakeFile과 setup.sh)
 ![image](https://user-images.githubusercontent.com/60434800/166079809-c1ea6c1f-eb0b-4541-a241-e6da53d77fb8.png)

MakeFile의 경우, 프로그램 통합 이전에 menu.c / menu2.c / form.c 각각을 테스트 하기 위해 해당 소스코드와 라이브러리들을 나눠서 관리했으며 현재는 다 통합되어 있는 상태이다.

![image](https://user-images.githubusercontent.com/60434800/166079816-4b31f3d0-1cd7-4c76-a6fc-accd429185fd.png)

setup.sh의 경우, 사용자가 소스코드와 기타 파일(ex. ticker_list.data)만을 가지고 있다고 가정하여 프로그램 내에서 필요한 폴더를 생성 후 데이터 파일을 옮겨주도록 스크립트가 설정 되어있다.

### 5.	README.md

 ![image](https://user-images.githubusercontent.com/60434800/166079846-a4cb1a17-69b1-4c82-9ebd-1c19b6ae7c6a.png)

소스 코드를 컴파일 하기 이전에 JSON-C를 받아서 링크를 설정해주어야 한다.
또한 순서대로 
$ ./setup.sh
$ make all
$ ./menu
실행 시키면 해당 프로그램을 이용할 수 있다.











### 동작 흐름도

![image](https://user-images.githubusercontent.com/60434800/166079854-440385d9-eb04-42e4-b164-ba7d5ee79d8e.png)

![image](https://user-images.githubusercontent.com/60434800/166079860-49672b17-f85a-40cc-be4d-de8985753595.png)

![image](https://user-images.githubusercontent.com/60434800/166079864-48004794-69cc-4bc9-b35b-b28c18196448.png)




 






 



#### 결과
### 데모 영상
https://www.youtube.com/watch?v=MG3dlx35C8A

### 사진 자료
  
![Screenshot from 2021-12-19 17-21-25](https://user-images.githubusercontent.com/60434800/166079914-6d42ef1b-b702-40ab-9174-992f32150f90.png)

![Screenshot from 2021-12-19 18-51-14](https://user-images.githubusercontent.com/60434800/166079917-ed2e2e56-a065-4322-bb20-36d81740a48e.png)

![Screenshot from 2021-12-19 18-51-18](https://user-images.githubusercontent.com/60434800/166079918-46deb5a5-f5a6-43be-9ad5-abdf8fcc226c.png)

![Screenshot from 2021-12-19 18-51-21](https://user-images.githubusercontent.com/60434800/166079920-cf7ca291-074a-4380-baba-55a7bd5e7a9c.png)

![Screenshot from 2021-12-19 18-51-50](https://user-images.githubusercontent.com/60434800/166079922-ae1edad5-8eb9-493d-a54c-1ea5d124e735.png)

![Screenshot from 2021-12-19 18-52-32](https://user-images.githubusercontent.com/60434800/166079924-69a463d6-99ef-4868-a08a-354925b7a924.png)

![Screenshot from 2021-12-19 18-52-40](https://user-images.githubusercontent.com/60434800/166079925-52b38960-eda9-40d7-874f-d608965cc3ed.png)

![Screenshot from 2021-12-19 18-52-44](https://user-images.githubusercontent.com/60434800/166079927-4b9c7bc5-f45d-4329-b654-a281e968b156.png)

![Screenshot from 2021-12-19 18-55-51](https://user-images.githubusercontent.com/60434800/166079929-dbccd5ee-6752-4f11-ab77-1f3c168fb90e.png)

![Screenshot from 2021-12-19 18-56-04](https://user-images.githubusercontent.com/60434800/166079933-284d7aa2-6c6c-4dc7-9399-cfe9c81ebde3.png)

  
 
 
 






### 참고 자료 
#### Tradier API
https://documentation.tradier.com/brokerage-api/overview/market-data

https://documentation.tradier.com/brokerage-api/markets/get-history

https://documentation.tradier.com/brokerage-api/markets/get-quotes
#### ncurses
https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/

https://pubs.opengroup.org/onlinepubs/7908799/xcurses/curses.h.html
#### json-c
https://github.com/json-c/json-c  
#### lib-curl API
https://curl.se/libcurl/c/  
