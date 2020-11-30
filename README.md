# BluType
![BluType](https://user-images.githubusercontent.com/60562619/100517829-0ebb1680-31d1-11eb-8fb6-40032708f94f.jpg)
BluType는 유선 키보드를 블루투스를 이용해 무선 키보드로 만들어 주는 장치입니다. 
Source Code: https://github.com/semteul/BluType

# 선정 이유
무선 키보드를 찾아보던 중 기존 멤브레인 키보드에 손이 익었기 때문에 이러한 조건의 키보드를 찾던 중이었습니다. 그러나 시중에 저렴한 멤브레인 키보드를 찾기는 쉽지 않은 상황이었습니다. 하지만 아두이노 등의 장치를 이용하면 가지고 있던 유선 키보드를 무선화 할 수 있지 않을까 하는 생각이 들었습니다. 그래서 키보드를 무선화 하기 위해 이 장치를 개발하게 되었습니다. 한편으로는 블루투스와 PS/2 프로토콜 등 통신 장비가 어떻게 작동하는지 배우고자 하는 목적도 일부 있었습니다. 

# 장치의 설명
개발 언어는 아두이노입니다. 
장치는 아래와 같은 구조로 구성되어있습니다.
 
**아두이노 보드 x 1
HID 블루투스 모듈(RN-42) x1
USB 커넥터 x 1**
USB 커넥터를 통해 키보드로부터 스캔코드를 입력받아 아두이노 보드가 이 코드를 해석해 HID 스캔코드로 변환한 뒤 블루투스 모듈을 통해 키보드 입력을 전달합니다.

다음은 BluType의 회로도입니다. 
![BluType Circuit](https://user-images.githubusercontent.com/60562619/100517857-37431080-31d1-11eb-8058-1c67e18e6f6d.png)

실제 연결한 경우 다음과 같습니다. (5v보조배터리 연결) 리튬배터리 및 충방전회로를 아두이노 micro, pro mini 등 작은 장치에 부착하면 장치를 훨씬 작게 소형화할 수 있습니다.
![BluType2](https://user-images.githubusercontent.com/60562619/100517832-0f53ad00-31d1-11eb-8075-971d1a643c81.jpg)

사용 방법은 아래와 같습니다.
* 유선 키보드를 USB 커넥터에 연결하고 장치의 전원을 켠다.
* 노트북/스마트폰 등 블루투스가 지원되는 장치에서 BluType를 찾는다.
* BluType과 장치를 연결한다.
* BluType의 버튼을 한번 누르면 전에 연결되었던 장치와 재연결을 시도한다.
* BluType의 버튼을 빠르게 두번 누르면 현재 연결된 장치와 연결을 끊는다.

# RN-42 모듈 초기화
사용 전에 모듈을 초기화 해야 할 필요가 있습니다. 이 프로젝트에서는 RN-42를 다음과 같이 초기화 시켰습니다.

***Settings***
BTA=201504295945
BTName=BluType
Baudrt=115K
Mode  =Slav
Authen=1
PinCod=1234
Bonded=0
Rem=1C232C8E31E6

***ADVANCED Settings***
SrvName= SPP
SrvClass=0000
DevClass=1F00
InqWindw=0100
PagWindw=0100
CfgTimer=255
StatuStr=%
HidFlags=2



# PS/2 프로토콜 
USB 방식의 통신은 제작사마다 통신방식이 다르기 때문에 입력을 받기가 쉽지 않습니다. 그러나 시중 대부분의 USB 키보드는 부가적으로 PS/2 프로토콜을 지원합니다. PS/2 프로토콜은 통일되어있으며, 간단하다는 이점이 있습니다. BluType에서는 이런 이유로 PS/2 프로토콜을 사용합니다. 즉 USB 커넥터로 연결을 해도, USB가 아닌 PS/2 프로토콜을 이용해 통신을 합니다. 이번 프로젝트에서는 다음 글들을 참조했습니다. 
[PS/2 키코드](https://wiki.osdev.org/PS/2_Keyboard)
[PS/2 프로토콜](http://www.burtonsys.com/ps2_chapweske.htm)

아두이노에는 이미 PS/2 프로토콜을 구현하는 라이브러리가 있었습니다. [[PS2KeyAdvanced](https://github.com/techpaul/PS2KeyAdvanced)] 그러나 이 라이브러리는 사용하지 않는 데이터 테이블과 기능들 때문에 무겁기 때문에 리소스가 매우 한정되어있는 아두이노에서 사용하기는 적합하지 않다고 생각했습니다. 그래서 PS/2 프로토콜을 입력받는 기능을 해당 라이브러리의 소스를 참고해 프로젝트의 상황에 맞추어 직접 구현하였습니다.

구현 방식은 아두이노의 인터럽트를 이용해서 PS/2 스캔코드의 신호가 들어올 때 마다 인터럽트를 실행해 8번 읽어 스캔코드를 읽어내는 방식을 사용했습니다. 실제 소스에서는 keyboard.h와 keyboard.ino가 이 부분을 담당합니다.
# HID 변환 테이블 작성
프로젝트에서 제일 시간이 많이 걸렸고 고민했던 부분은 PS/2 프로토콜에서 HID 변환 테이블을 작성하는 방식이었습니다. 두 통신방법은 꽤 많은 부분이 다르기 때문에 1대1로 치환하는 변환 테이블 하나만 가지고 구현할 수는 없었습니다. PS/2 프로토콜은 스캔코드 여러개를 조합해서 하나의 키와 상태를 모두 나타내는 방식입니다. 그 반면에 HID는 스캔코드 하나가 한개의 키를 나타내며, 출력 버퍼에 눌린 상태를 나타내고 싶은 키코드를 입력함으로서 키가 눌린것을 표현하는 방식입니다. 그래서 PS/2 프로토콜의 스캔코드를 상황별로 쪼개서 HID 스캔코드로 변환하는 방식으로 만들었습니다. HID 변환 테이블의 대한 참조는 RN-42의 데이터시트를 참조했습니다. 
[[RN-42 데이터 시트]](https://cdn.sparkfun.com/datasheets/Wireless/Bluetooth/bluetooth_cr_UG-v1.0r.pdf)

HID의 변환 테이블은 자바스크립트를 이용해 자동으로 매칭하는 방식으로 생성했습니다. 해당 소스파일과 변환 테이블의 csv 파일을 레포지토리에 업로드했습니다. 

![HID 변환 코드 예시](https://user-images.githubusercontent.com/60562619/100517937-bfc1b100-31d1-11eb-9bf1-5400bcdf0eda.png)
[ HID 변환 코드 예시 ] 
# HID 전송
HID는 한번에 최대 6개까지의 키를 전송할 수 있습니다. 하지만 HID에는 Modifier를 이용하면 컨트롤,쉬프트,알트,GUI(윈도우키)를 Modifier에 한 비트씩 할당하여 보내는 기능이 있는데, 이를 이용하면 자주 사용하는 기능키를 6개의 키 제한에서 제외할 수 있다는 큰 장점이 있습니다. 그래서 이 기능 또한 구현했습니다. 
![image](https://user-images.githubusercontent.com/60562619/100518433-c7cf2000-31d4-11eb-9da2-48fb7c1ad803.png)
[ HID의 Modifier 비트 ]

![image](https://user-images.githubusercontent.com/60562619/100518423-bbe35e00-31d4-11eb-975d-47e910ca520f.png)
[ 실제 사용중인 HID의 Modifier와 ScanCode Buffer가 어떻게 채워지는지 출력 ]

추가적으로 한자, 한/영키 등은 PS/2 키보드에서 눌린 경우에만 스캔코드가 존재하고 떼어진 경우에 대한 스캔코드가 할당이 되어있지 않습니다. 그러나 HID에서는 키가 떼어진 상황 또한 고려해야 하기 때문에 이런 키들은 키 눌림을 입력받았을 때 자동적으로 바뀌도록 했습니다.

# 아쉬웠던 점
본래 이 프로젝트의 최종 목적은 멀티 페어링이 가능한 장치를 만들고자 하는 것이었습니다. 본래 구상은 버튼을 누르면 자동으로 다른 기기와 연결 되는 기능이었습니다. 그러나 현재 가지고 있는 RN-42 모듈의 펌웨어 버전에서는 임의적으로 페어링 할 경우 연결이 끊어져버리는 버그가 발생하고 있습니다. 그렇기 때문에 멀티 페어링 기능이 실제 동작하는지는 확인하지 못했습니다. 다만 구현한 소스코드는 남아있으며, 추후에 새로운 버전의 모듈을 이용해 소스코드를 확인해볼 생각입니다.

# 어려웠던 점 및 소감
위에서 설명했던 변환 테이블 이 외에도, 여러가지의 장치를 동시에 입출력을 받다보니 객체화, 병렬화를 이용해서 코딩해야 했던 점이 어려웠습니다. 게다가 아두이노의 리소스가 한정되어있다보니, 변환 테이블을 크게 선언해서 메모리가 초과되거나, 속도가 느려 타이밍이 엉망이 되는 경우도 많았습니다.  그러나 실제로 작동하는 장치를 만들 수 있었고, 하드웨어 통신을 처음 직접 다뤄보면서 프로토콜의 원리를 배울 수 있었기 때문에 무척 알차고 재미있었습니다. 

# 추가할 기능
1. 통신 오류 처리 - PS/2 프로토콜과의 통신 과정에서 오류를 검출한다면 다시 전송 요청 / 블루투스 모듈과 UART 통신중 이상 발생시 블루투스 모듈 리부트
2. 다중 페어링 - 새로운 버전의 RN-42를 구하게 된다면 다중 페어링기능을 테스트해서 잘 작동한다면 다중 페어링 기능을 포함해서 정식 버전으로 릴리즈하기
3. 저전력 기능 - 오랫동안 장치를 사용하지 않는 경우, 장치를 저전력 모드로 두기
4. 마우스 지원 - 키보드 뿐만 아니라 마우스 지원
