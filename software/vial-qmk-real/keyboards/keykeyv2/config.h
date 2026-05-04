#pragma once

// 항상 왼쪽에 USB를 연결하면 왼쪽=마스터, 오른쪽=슬레이브로 고정됨
// 하드웨어 핀 배선 불필요 (GP29 미사용)
#define MASTER_LEFT

// ── 대안: 특정 핀에 저항으로 핸드니스 감지 ──────────────────────────────
// 왼쪽 보드: 해당 핀에 풀업 저항 달기 (핀이 HIGH → 왼쪽)
// 오른쪽 보드: 해당 핀을 GND에 연결 (핀이 LOW  → 오른쪽)
// 사용 방법: MASTER_LEFT 주석 처리 후 아래 두 줄 활성화
//
// #define SPLIT_HAND_PIN GP29
// #define SPLIT_USB_DETECT
//
// (SPLIT_USB_DETECT 없이 SPLIT_HAND_PIN만 써도 되지만,
//  TRRS로 5V 공유 시 양쪽이 마스터로 오인할 수 있어 함께 쓰는 게 안전)
// ─────────────────────────────────────────────────────────────────────────
// 통신이 불안정할 때 슬레이브가 멈춰있지 않게 리셋/재시도
#define SPLIT_WATCHDOG_ENABLE

// Custom split RPC: sync 5-way nav GPIO state from the other half to master.
#define SPLIT_TRANSACTION_IDS_USER NAV5_GPIO_SYNC

#define DIRECT_KEY_LEFT   GP12
#define DIRECT_KEY_DOWN   GP13
#define DIRECT_KEY_UP     GP14
#define DIRECT_KEY_RIGHT  GP15

#define LED_CAPS_LOCK_PIN GP28
#define LED_PIN_ON_STATE  1

#define TAPPING_TERM 200

// Split serial (RP2040 vendor/Pio) - Full-duplex로 전환
// 기존 GP0 라인은 유지하고, 추가 라인 1개(GP26)를 더 써서 RX/TX를 분리한다.
// 배선은 "동일 핀끼리 직결" (GP0<->GP0, GP26<->GP26)로 하고,
// 마스터 쪽만 TX/RX 역할을 뒤집기 위해 SERIAL_USART_PIN_SWAP을 사용.
#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_PIN_SWAP
// 키보드 JSON(split.serial.pin=GP0)에서 SOFT_SERIAL_PIN이 정의되므로
// TX는 SOFT_SERIAL_PIN(GP0)을 그대로 사용하고, RX만 별도로 지정한다.
#define SERIAL_USART_RX_PIN GP26

// 안정성 우선으로 속도는 중간부터 시작(필요하면 올려도 됨)
#define SERIAL_USART_SPEED 57600

// ── Vial 필수 설정 ──────────────────────────────────────────────────────────
// python3 util/vial_generate_keyboard_uid.py 로 생성된 고유 ID
#define VIAL_KEYBOARD_UID {0x04, 0x68, 0x66, 0x41, 0x49, 0xED, 0xC3, 0x78}

// Vial 잠금 해제 콤보: 좌측 Ctrl(row=4,col=0) + 우측 Backspace(row=9,col=5)
#define VIAL_UNLOCK_COMBO_ROWS {4, 9}
#define VIAL_UNLOCK_COMBO_COLS {0, 5}
// ─────────────────────────────────────────────────────────────────────────────
