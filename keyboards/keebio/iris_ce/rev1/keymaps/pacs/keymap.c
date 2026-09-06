/* Iris CE (keebio/iris_ce) — 판독 배치  [시험안 2026-09-06]
 * 기준 문서: docs/IRIS_CE_LAYOUT.md
 *
 * 엄지 (편의 순위)
 *   왼  1위 BSpc(rest, 순수 tap) / 2위 ] hold=LAlt / 3위 F14(순수 tap) / row4 SAT hold=LGui
 *   오른 1위 Space(rest) hold=FN / 2위 Enter(순수 tap) / 3위 F13 hold=LCtrl / row4 한영 hold=RShift
 *
 * 2026-09-06: FN 을 오른엄지 rest(Space)에 겹친다.
 *   레이어 내용이 왼손에 몰려 있다(화살표 391/일, Home/End, 단어·행 삭제).
 *   FN 이 오른엄지면 교차손이 되고, rest 라 잡는 자세도 가장 편하다.
 *
 *   rest 에 hold 를 얹지 않는다는 원칙은 '모디파이어' 기준이었다. layer-tap 은
 *   오발동해도 화살표가 나갈 뿐이라 즉시 보이고 무해하다. Ctrl+S(저장)·
 *   Ctrl+W(닫기) 처럼 되돌리기 어려운 결과가 없다.
 *
 *   그 결과 Alt 이 ](94/일)로 내려가고 Enter(378/일)는 순수 tap 이 된다.
 *   378 -> 94 로 mod-tap 오판정 노출이 4배 줄었다.
 *
 * ⚠ 대가: 레이어 진입에 tapping term(220ms) 만큼 기다려야 한다. tap-preferred 라
 *   Space 를 그만큼 잡은 뒤에야 화살표가 먹는다. 버스트에서는 첫 진입만 지연되지만
 *   한 번만 누를 때는 체감된다. 견디기 어려우면 이 값을 먼저 낮춘다.
 *
 *   XR 연속 판독의 핵심 루프 ] → F14 는 둘 다 왼엄지지만 키3(rest 옆) → 키1 이라
 *   rest 를 지나는 완만한 이동이다.
 *
 * Space는 순수 tap을 유지한다. Enter는 tap 뒤 즉시 문자가 이어지는 경우가 드물어
 * Alt hold를 받으며, F14는 오판정 위험 없는 순수 확정키로 둔다.
 *
 * ⚠ F14 = Alt+F(normal 확인), F15 = Alt+G(이전복사+확인).
 *   뒤집어 넣으면 확인 대신 이전 리포트를 붙여넣고 저장한다.
 *
 * ⚠ 부트로더 진입: FN(오른엄지 Space) hold + 홈로우 맨 바깥 키.
 *   왼쪽 = 'A' 왼쪽(Ctrl 자리), 오른쪽 = ';' 오른쪽(' 자리). 좌우 각각 진입한다.
 *   이게 없으면 플래시 후 물리 리셋 버튼 말고는 되돌릴 방법이 없다.
 *
 * 한/영은 오른쪽 row4 여분의 RShift mod-tap, ]는 오른엄지 FN layer-tap이다.
 */

#include QMK_KEYBOARD_H

enum layers { _BASE = 0, _FN };

enum custom_keycodes {
    WDEL_B = SAFE_RANGE,  /* 앞단어 삭제 */
    WDEL_F,               /* 뒤단어 삭제 */
    LDEL_U,               /* 윗행 삭제   */
    LDEL_C,               /* 현재행 삭제 */
    LDEL_D,               /* 아랫행 삭제 */
    COPY_ONCE,            /* X+C combo → Ctrl+C */
    PASTE_ONCE,           /* C+V combo → Ctrl+V */
    SAT_GUI,              /* tap=Super Alt-Tab / hold=Win */
};

enum combo_events {
    COPY_XC,
    PASTE_CV,
};

const uint16_t PROGMEM copy_xc_combo[]  = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM paste_cv_combo[] = {KC_C, KC_V, COMBO_END};

combo_t key_combos[] = {
    [COPY_XC]  = COMBO(copy_xc_combo, COPY_ONCE),
    [PASTE_CV] = COMBO(paste_cv_combo, PASTE_ONCE),
};

/* 마우스 선택 후의 입력 공백에서만 콤보를 허용한다.
 * 일반 타이핑 도중의 X-C/C-V 롤은 직전 입력 120ms 조건으로 차단한다. */
static uint16_t combo_last_press_timer = 0;
static bool     combo_have_last_press  = false;
static bool     combo_idle_armed       = false;

static bool is_copy_paste_combo_key(uint16_t keycode) {
    return keycode == KC_X || keycode == KC_C || keycode == KC_V;
}

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return true;

    uint16_t elapsed = combo_have_last_press ? timer_elapsed(combo_last_press_timer) : 0xFFFF;
    if (is_copy_paste_combo_key(keycode)) {
        if (elapsed >= 120) {
            combo_idle_armed = true;
        } else if (!(combo_idle_armed && elapsed <= 40)) {
            combo_idle_armed = false;
        }
    } else {
        combo_idle_armed = false;
    }

    combo_last_press_timer = timer_read();
    combo_have_last_press  = true;
    return true;
}

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    (void)combo_index;
    (void)combo;
    (void)keycode;
    (void)record;
    return get_highest_layer(layer_state | default_layer_state) == _BASE && combo_idle_armed;
}

/* 엄지 */
#define TH_RBRC_ALT LALT_T(KC_RBRC) /* ] tap / Alt hold     — 왼 2위 */
#define TH_SPC_FN LT(_FN, KC_SPC)  /* Space tap / FN hold  — 오른 1위 rest */
#define TH_F13  LCTL_T(KC_F13)     /* F13 tap / Ctrl hold  — 오른 3위 */
#define TH_HAN_RSFT RSFT_T(KC_RALT) /* 한영 tap / RShift hold — 오른 4위 (보조 Shift) */
#define SFT_OS  OSM(MOD_LSFT)      /* 왼쪽 외곽열 sticky shift (유지) */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* ── BASE ────────────────────────────────────────────────────────────────
 *  Esc(GESC)  1  2  3  4  5        6  7  8  9  0  -
 *  Tab        Q  W  E  R  T        Y  U  I  O  P  \
 *  Ctrl       A  S  D  F  G        H  J  K  L  ;  '
 *  Shift      Z  X  C  V  B  SAT 한영   N  M  ,  .  /  Shift
 *                  F14 BSpc ]  Enter Space F13   (Space=FN hold)
 *  Esc = Grave Escape :  Esc / Shift+Esc = ~ / Win+Esc = `
 */
[_BASE] = LAYOUT(
  QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
  KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  SFT_OS,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,   SAT_GUI, TH_HAN_RSFT, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
                             KC_F14,  KC_BSPC, TH_RBRC_ALT,      KC_ENT,  TH_SPC_FN, TH_F13
),

/* ── FN (오른엄지 Space hold) ─────────────────────────────────────────────
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
 *  ⚠ 엄지 '*' 예외는 그대로 필요하다. Alt(왼엄지 ])+왼손 글자가 같은 손이다.
 *    FN 자체는 오른엄지라 왼손 화살표와 교차손이다. 아래 chordal_hold_layout 참조.
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
 * Alt(왼엄지 ]) + 왼손 글자가 여기 걸린다. FN(오른엄지 Space)+왼손 화살표는 교차손이다.
 * Alt+Tab 은 SAT_GUI 매크로가 처리하므로 이 규칙과 무관하다.
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
 * Enter/Alt와 F13/Ctrl은 200ms로 tap/hold를 구분한다.
 */
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TH_SPC_FN:  return 220; /* 레이어 진입: Space tap 오발동 방지 */
        case TH_F13:     return 200; /* 확정 동사: 오발동 시 되돌릴 수 없어 여유를 둔다 */
        case TH_RBRC_ALT: return 200; /* ] tap과 Alt hold를 안정적으로 구분 */
        default:      return 180;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TH_F13:
        case TH_RBRC_ALT:
        case TH_HAN_RSFT:
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
static bool     is_alt_tab_active = false;
static uint16_t alt_tab_timer     = 0;
static bool     sat_gui_down      = false;
static bool     sat_gui_held      = false;
static uint16_t sat_gui_timer     = 0;

/* Alt 순환 중 Shift 는 Alt 을 풀지 않는다 — Alt+Shift+Tab 역방향용. */
static bool is_shift_key(uint16_t keycode) {
    switch (keycode) {
        case KC_LSFT:
        case KC_RSFT:
        case SFT_OS:
        case TH_HAN_RSFT:
            return true;
        default:
            return false;
    }
}

static void release_super_alt_tab(void) {
    if (is_alt_tab_active) {
        unregister_code(KC_LALT);
        is_alt_tab_active = false;
    }
}

void matrix_scan_user(void) {
    /* 단독 hold가 280ms를 넘으면 실제 Win key-down으로 전환한다.
     * 180ms는 창 순환 연타 중 한 번만 늦어도 시작 메뉴가 떠서 순환이 끊겼다.
     * Win 단독은 거의 쓰지 않으므로 tap 쪽에 여유를 준다. */
    if (sat_gui_down && !sat_gui_held && timer_elapsed(sat_gui_timer) >= 280) {
        release_super_alt_tab();
        register_code(KC_LGUI);
        sat_gui_held = true;
    }

    if (is_alt_tab_active && timer_elapsed(alt_tab_timer) > 1000) {
        release_super_alt_tab();
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    /* 전용 dual-role: QMK mod-tap의 KC_NO tap 후처리에 의존하지 않는다. */
    if (keycode == SAT_GUI) {
        if (record->event.pressed) {
            sat_gui_down  = true;
            sat_gui_held  = false;
            sat_gui_timer = timer_read();
        } else {
            if (sat_gui_held) {
                unregister_code(KC_LGUI);
            } else {
                /* 짧은 tap: 공식 Super Alt-Tab 패턴 */
                if (!is_alt_tab_active) {
                    register_code(KC_LALT);
                    is_alt_tab_active = true;
                }
                alt_tab_timer = timer_read();
                tap_code(KC_TAB);
            }
            sat_gui_down = false;
            sat_gui_held = false;
        }
        return false;
    }

    /* SAT_GUI를 누른 채 다른 키를 누르면 즉시 Win hold로 확정한다. */
    if (sat_gui_down && !sat_gui_held && record->event.pressed) {
        release_super_alt_tab();
        register_code(KC_LGUI);
        sat_gui_held = true;
    }

    /* 다른 키를 누르면 Alt를 먼저 놓고 그 키는 정상 처리한다.
     * Shift 만 예외다. 순환 중 Shift 를 잡고 SAT 을 다시 치면 Alt+Shift+Tab 이
     * 나가 역방향으로 돈다(Windows 기본 동작과 동일). */
    if (is_alt_tab_active && record->event.pressed && !is_shift_key(keycode)) {
        release_super_alt_tab();
    }

    if (!record->event.pressed) return true;

    switch (keycode) {
        case COPY_ONCE:
            tap_code16(C(KC_C));
            return false;

        case PASTE_ONCE:
            tap_code16(C(KC_V));
            return false;

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
