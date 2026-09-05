/* Iris CE (keebio/iris_ce) — 판독 배치  [시험안 2026-09-06]
 * 기준 문서: docs/IRIS_CE_LAYOUT.md
 *
 * 엄지 (편의 순위)
 *   왼  1위 BSpc(rest) / 2위 Enter(hold 없음) / 3위 F14 hold=LAlt / 4위 ] hold=LGui
 *   오른 1위 Space(rest) / 2위 한영 hold=FN / 3위 F13 hold=LCtrl / 4위 [ hold=RShift
 *
 * 2026-09-06 시험안: Enter·편집 편의를 우선한다.
 *   Enter 2위(순수 tap) + 오른엄지 FN 2위(왼손 편집키를 교차손으로 받침).
 *   확정 동사 F13·F14 는 같은 3위 등급. 한/영은 오른손 습관 유지.
 *   검증할 것: Enter·FN 개선이 Alt·Ctrl 의 3순위 부담을 상쇄하는가.
 *
 * ⚠ Space / Enter 에는 hold 를 얹지 않는다.
 *   683/일 · 378/일 로 최다 구간이라 mod-tap 오판정 노출이 과도하다.
 *
 * ⚠ F14 / F13 은 hold 오판정 위험이 남는다.
 *   Alt(또는 Ctrl) 의도가 tap 으로 판정되면 확정 동사가 나간다. 되돌릴 수 없다.
 *   modifier 종류를 바꾸는 것으로는 해결되지 않는다 — tapping term 실측으로만 줄인다.
 *
 * ⚠ F14 = Alt+F(normal 확인), F15 = Alt+G(이전복사+확인).
 *   뒤집어 넣으면 확인 대신 이전 리포트를 붙여넣고 저장한다.
 *
 * ⚠ 부트로더 진입: FN(오른엄지 한/영) hold + 홈로우 맨 바깥 키.
 *   왼쪽 = 'A' 왼쪽(Ctrl 자리), 오른쪽 = ';' 오른쪽(' 자리). 좌우 각각 진입한다.
 *   이게 없으면 플래시 후 물리 리셋 버튼 말고는 되돌릴 방법이 없다.
 *
 * ⚠ LT(_FN, KC_RALT) 는 모디파이어를 tap 에 넣는 형태라 QMK 공식 지원이 아니다.
 *   한/영 tap 이 안 나오면 KC_LNG1 로 바꾸거나 커스텀 키코드로 구현할 것.
 */

#include QMK_KEYBOARD_H

enum layers { _BASE = 0, _FN };

enum custom_keycodes {
    WDEL_B = SAFE_RANGE,  /* 앞단어 삭제 */
    WDEL_F,               /* 뒤단어 삭제 */
    LDEL_U,               /* 윗행 삭제   */
    LDEL_C,               /* 현재행 삭제 */
    LDEL_D,               /* 아랫행 삭제 */
};

/* 엄지 */
#define TH_HAN  LT(_FN, KC_RALT)   /* 한영 tap / FN hold   — 오른 2위 */
#define TH_F14  LALT_T(KC_F14)     /* F14 tap / Alt hold   — 왼 3위 (F13 과 동일 등급) */
#define TH_RBRC LGUI_T(KC_RBRC)    /* ] tap   / Win hold   — 왼 4위 */
#define TH_F13  LCTL_T(KC_F13)     /* F13 tap / Ctrl hold  — 오른 3위 */
#define TH_LBRC RSFT_T(KC_LBRC)    /* [ tap   / Shift hold — 오른 4위 (보조 Shift) */
#define SFT_OS  OSM(MOD_LSFT)      /* 왼쪽 외곽열 sticky shift (유지) */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* ── BASE ────────────────────────────────────────────────────────────────
 *  Esc(GESC)  1  2  3  4  5        6  7  8  9  0  -
 *  Tab        Q  W  E  R  T        Y  U  I  O  P  \
 *  Ctrl       A  S  D  F  G        H  J  K  L  ;  '
 *  Shift      Z  X  C  V  B  ]   [   N  M  ,  .  /  Shift
 *                  F14 BSpc Enter  한영 Space F13
 *  Esc = Grave Escape :  Esc / Shift+Esc = ~ / Win+Esc = `
 */
[_BASE] = LAYOUT(
  QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
  KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  SFT_OS,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,   TH_RBRC,  TH_LBRC, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                             TH_F14,  KC_BSPC, KC_ENT,           TH_HAN,  KC_SPC,  TH_F13
),

/* ── FN (왼엄지 한/영 hold) ──────────────────────────────────────────────
 *  왼손  숫자행 2 3 4 = 윗행/현재행/아랫행 삭제        ← 행 단위
 *        W E R T      = Home PgDn PgUp End            ← 줄/페이지
 *        S D F G      = ←  ↓  ↑  →                    ← 문자 (A' 배열)
 *        X C V B      = C+← 앞단어삭제 뒤단어삭제 C+→  ← 단어
 *        A = Caps Word,  Z = CapsLock,  row3 여분 = Delete
 *        홈로우 외곽열 좌우 = QK_BOOT (부트로더) — Glove80 의 magic+홈로우외곽 과 같은 자리
 *
 *  오른손 숫자행 = F15 F16 F17 F18 F19,  - 자리 = '='
 *        U I O P \ = F7  F8  F9  F10 F11
 *        J K L ;   = F4  F5  F6  F12
 *        M , .     = F1  F2  F3
 *
 *  ⚠ 엄지 '*' 예외는 그대로 필요하다. FN(오른엄지)+오른손 F키, Alt(왼엄지)+왼손 Tab
 *    이 여전히 같은 손 조합이다. 아래 chordal_hold_layout 참조.
 */
[_FN] = LAYOUT(
  _______, _______, LDEL_U,  LDEL_C,  LDEL_D,  _______,                   KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_EQL,
  _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,                    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
  QK_BOOT, CW_TOGG, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,                   _______, KC_F4,   KC_F5,   KC_F6,   KC_F12,  QK_BOOT,
  _______, KC_CAPS, LCTL(KC_LEFT), WDEL_B, WDEL_F, LCTL(KC_RGHT), KC_DEL,
                                                          _______, _______, KC_F1,   KC_F2,   KC_F3,   _______, _______,
                             _______, _______, _______,           _______, _______, _______
)
};

/* ── Chordal Hold 손 지정 ────────────────────────────────────────────────
 * Chordal Hold 는 "반대손 조합만 hold" 규칙이라 같은 손 조합을 tap 으로 settle 한다.
 * FN(오른엄지) + 오른손 F키, Alt(왼엄지) + Tab(왼쪽 외곽열) 이 여기 걸린다.
 * 특히 Alt+Tab 은 231/일 이라 예외가 없으면 사실상 못 쓴다.
 * 엄지와 row3 여분을 '*' 로 두면 이 규칙에서 면제된다.
 *   https://docs.qmk.fm/tap_hold#chordal-hold
 */
#ifdef CHORDAL_HOLD
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT(
  'L','L','L','L','L','L',                 'R','R','R','R','R','R',
  'L','L','L','L','L','L',                 'R','R','R','R','R','R',
  'L','L','L','L','L','L',                 'R','R','R','R','R','R',
  'L','L','L','L','L','L','*',        '*','R','R','R','R','R','R',
                  '*','*','*',             '*','*','*'
);
#endif

/* ── tap-hold 튜닝 ───────────────────────────────────────────────────────
 * F13 / F14 는 확정 키라 연타하지 않는다(사용자 확인) → hold 인식을 우선한다.
 * 다만 hold 오판정이 확정으로 이어지므로, 오발동이 나면 이 두 키의
 * tapping term 을 먼저 올린다.
 */
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TH_HAN:  return 220;   /* 레이어 진입: 한영 tap 오발동 방지 */
        case TH_F13:
        case TH_F14:  return 200;   /* 확정 동사: 오발동 시 되돌릴 수 없어 여유를 둔다 */
        default:      return 180;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TH_F13:
        case TH_F14:
        case TH_RBRC:
        case TH_LBRC:
            return true;            /* 모디파이어는 빠르게 확정 */
        default:
            return false;           /* 레이어 키는 tap-preferred */
    }
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    return 0;                       /* 자동반복 미사용(사용자 확인) → hold 판정 안정 */
}

/* ── 편집 매크로 ─────────────────────────────────────────────────────────
 * 날 Ctrl+Backspace 를 쓰지 않는 이유: PACS 리포트 필드는 표준 Win32 Edit 이라
 * 제어문자(^H)를 삽입할 수 있다. 선택 후 Delete 로 우회한다.
 *
 * ⚠ 워드랩이 켜져 있으면 ↑/↓ 가 논리 줄이 아니라 화면 줄로 움직인다.
 *   긴 소견이 접혀 있으면 일부만 지워진다. 실제 편집기에서 확인할 것.
 */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return true;

    switch (keycode) {
        case WDEL_B:   /* 앞단어 삭제 : Ctrl+Shift+←  →  Del */
            SEND_STRING(SS_LCTL(SS_LSFT(SS_TAP(X_LEFT))) SS_DELAY(12) SS_TAP(X_DELETE));
            return false;

        case WDEL_F:   /* 뒤단어 삭제 : Ctrl+Shift+→  →  Del */
            SEND_STRING(SS_LCTL(SS_LSFT(SS_TAP(X_RIGHT))) SS_DELAY(12) SS_TAP(X_DELETE));
            return false;

        case LDEL_C:   /* 현재행 삭제 : Home → Shift+↓ → Del */
            SEND_STRING(SS_TAP(X_HOME) SS_DELAY(12)
                        SS_LSFT(SS_TAP(X_DOWN)) SS_DELAY(12)
                        SS_TAP(X_DELETE));
            return false;

        case LDEL_U:   /* 윗행 삭제 : Home → Shift+↑ → Del */
            SEND_STRING(SS_TAP(X_HOME) SS_DELAY(12)
                        SS_LSFT(SS_TAP(X_UP)) SS_DELAY(12)
                        SS_TAP(X_DELETE));
            return false;

        case LDEL_D:   /* 아랫행 삭제 : End → Shift+↓ → Shift+End → Del
                        * Shift 를 놓지 않고 두 번 확장해야 선택이 유지된다. */
            SEND_STRING(SS_TAP(X_END) SS_DELAY(12)
                        SS_DOWN(X_LSFT) SS_DELAY(8)
                        SS_TAP(X_DOWN) SS_DELAY(8)
                        SS_TAP(X_END) SS_DELAY(8)
                        SS_UP(X_LSFT) SS_DELAY(12)
                        SS_TAP(X_DELETE));
            return false;
    }
    return true;
}
