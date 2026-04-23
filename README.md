# keykeyv2 빌드 가이드

---

## 키보드 특징

- **스플릿 키보드** (좌/우 분리형)
- **각 사이드 5행 6열** = 총 60키 (매트릭스)
- **방향키 4개** × 2사이드 (다이렉트 핀, 5방향 네비게이션 스위치)
- **캡스락 LED** 1개 × 2사이드 (GP28)
- **PCB 앞면 = 좌측 / 뒷면 = 우측** (단일 PCB 양면 활용)
- **TRRS 싱글 와이어 통신** (GP0)
- **Vial-QMK** 사용 (GUI로 키맵 변경 가능)

---

## 핀 배치

| 용도 | 핀 |
|---|---|
| 통신 (TRRS) | GP0 |
| ROW 0~4 | GP1, GP2, GP3, GP4, GP5 |
| COL 0~5 | GP6, GP7, GP8, GP9, GP10, GP11 |
| 방향키 (UP/DOWN/LEFT/RIGHT) | GP12, GP13, GP14, GP15 |
| 캡스락 LED | GP28 |

---

## 필요 부품

### 🔲 프로세서
| 항목 | 수량 | 상세 |
|---|---|---|
| RP2040-Zero | 2개 | 좌/우 각 1개. 옆면 캐스텔레이티드(Castellated) 핀 납땜 계획. USB-C 내장 |

### ⌨️ 스위치 & 소켓
| 항목 | 수량 | 상세 |
|---|---|---|
| Kailh Choc v1 (1350) 스위치 | 90개 | 로우 프로파일 기계식. 5핀 권장 (안정성↑). 각 사이드 45개 |
| Choc v1 전용 핫스왑 소켓 | 90개 | 반드시 Choc v1 전용 규격 확인 (MX 소켓과 호환 안됨) |

### 🔵 다이오드
| 항목 | 수량 | 상세 |
|---|---|---|
| 1N4148W SMD 다이오드 | 95개 | SOD-123 패키지. 여분 5개 포함. 캐소드(|) 방향을 COL 쪽으로 |

### 🕹️ 방향키 & 인코더
| 항목 | 수량 | 상세 |
|---|---|---|
| 5-Way 네비게이션 스위치 | 1개 | 상/하/좌/우/센터 클릭 지원 모델. PCB에 45도 회전 배치 권장 |
| EC11 로터리 인코더 | 1개 | D-Cut 축, 15mm 길이 권장. 노브 별도 구매 필요 |

> ⚠️ 5방향 스위치를 정사각형으로 배치하면 대각 방향으로 눌러야 상하좌우 동작함.
> PCB 설계 시 반드시 **45도 회전(마름모) 배치** 권장

### 💡 상태 표시 LED
| 항목 | 수량 | 상세 |
|---|---|---|
| 3mm LED | 2개 | 각 사이드 1개. Caps Lock 활성 표시용 |
| 저항 330Ω | 2개 | LED 보호용. 각 사이드 GP28과 LED 사이에 직렬 연결 |

### 🔌 통신 & 연결
| 항목 | 수량 | 상세 |
|---|---|---|
| PJ-320A TRRS 잭 | 2개 | 반드시 4극 전용 (3극 사용 시 쇼트 위험) |
| 4극 TRRS 케이블 | 1개 | 양쪽 유선 연결용 |
| 저항 1kΩ | 2개 | GP0 ↔ TRRS Ring 사이 각 사이드 1개씩 (통신 보호용) |

### TRRS 배선
```
Left RP2040                        Right RP2040
GP0 ──[1kΩ]── TRRS Ring ──[1kΩ]── GP0
GND ────────── TRRS Sleeve ──────── GND
VBUS ───────── TRRS Tip ──────────── VBUS
```

### LED 배선
```
GP28 ──[330Ω]── LED(+) ── LED(-) ── GND
```

---

## 필요 프로그램 설치

### 1. Homebrew (미설치 시)
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### 2. QMK CLI
```bash
brew install qmk/qmk/qmk
```

### 3. vial-qmk 클론
```bash
git clone https://github.com/vial-kb/vial-qmk.git
cd vial-qmk
make git-submodule
```

### 4. QMK 환경 설정
```bash
qmk config user.qmk_home=/Users/유저명/workspace/vial-qmk
qmk setup
```

### 5. Vial GUI
- https://get.vial.today 에서 다운로드

---

## MCU 부트로더 모드 연결 방법

```
방법 1: BOOT 버튼을 누른 채로 USB 연결
방법 2: BOOT 버튼 누르면서 RESET 버튼 누르기
방법 3: keymap에서 QK_BOOT 키 입력 (펌웨어 설치 후)
```

부트로더 모드 진입 시 Mac Finder에 **RPI-RP2** 드라이브로 마운트됨

---

## 컴파일 방법

```bash
cd /Users/유저명/workspace/vial-qmk
qmk compile -kb keykeyv2 -km vial
```

컴파일된 파일 위치:
```
vial-qmk/.build/keykeyv2_vial.uf2
```

---

## 컴파일 클리어 방법

```bash
qmk clean -kb keykeyv2
```

클리어 후 재컴파일:
```bash
qmk clean -kb keykeyv2 && qmk compile -kb keykeyv2 -km vial
```

---

## 좌/우 플래시 방법

> EE_HANDS 방식 사용 - 좌/우 MCU에 반드시 각각 다른 명령으로 플래시

### 왼쪽 MCU
```bash
qmk flash -kb keykeyv2 -km vial -bl uf2-split-left
```

### 오른쪽 MCU
```bash
qmk flash -kb keykeyv2 -km vial -bl uf2-split-right
```

---

## 수동 .uf2 업로드 방법

자동 플래시가 안 될 경우:

1. RP2040-Zero를 부트로더 모드로 연결
2. Finder에 **RPI-RP2** 드라이브 나타남
3. `.build/keykeyv2_vial.uf2` 를 RPI-RP2 드라이브에 드래그앤드롭
4. 자동 재시작 후 키보드로 인식됨

> ⚠️ 수동 업로드는 좌/우 구분이 안되므로 반드시 `qmk flash` 명령 사용 권장

---

## 좌/우 지정 확인 방법

1. TRRS 케이블 분리
2. 확인할 MCU만 USB 연결
3. 키 입력 테스트
   - 입력 정상 동작 → 올바르게 지정됨 ✅
   - 입력 안됨 → 반대로 지정됨 → 재플래시 필요 ❌
