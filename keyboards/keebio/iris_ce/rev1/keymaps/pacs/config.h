/* Iris CE — 판독 배치 config.h
 * 2026-09-05.  기준 문서: docs/IRIS_CE_LAYOUT.md
 *
 * 세부 값은 keymap.c 의 get_tapping_term / get_permissive_hold /
 * get_quick_tap_term 에서 키별로 덮어쓴다. 여기는 기본값과 기능 스위치만.
 */

#pragma once

/* ── tap-hold 기본값 ────────────────────────────────────────────────────
 * 확정 동사(F13/F14)는 연타하지 않으므로 hold 인식을 우선한다.
 * QUICK_TAP_TERM 0 = tap 직후 hold 로 자동반복하지 않음 → hold 판정이 안정.
 * cine(연속 이미지 이동)은 ShuttlePro 셔틀링이 담당하므로 자동반복이 필요 없다.
 */
#define TAPPING_TERM 180
#define QUICK_TAP_TERM 0
#define PERMISSIVE_HOLD          /* keymap.c 에서 키별로 다시 끈다 */
#define TAPPING_TERM_PER_KEY    /* get_tapping_term() is enabled below */
#define PERMISSIVE_HOLD_PER_KEY /* get_permissive_hold() is enabled below */
#define QUICK_TAP_TERM_PER_KEY  /* get_quick_tap_term() is enabled below */

/* 같은 손 롤로 인한 mod-tap 오발동 억제.
 * 홈로우 모드를 쓰지 않으므로 노출은 작지만, 엄지 mod-tap 에도 도움이 된다. */
#define CHORDAL_HOLD

/* ── 왼손 Copy/Paste 콤보 ──────────────────────────────────────────────
 * X+C = Copy, C+V = Paste. Base 레이어에서만 동작하며 keymap.c의
 * combo_should_trigger()가 직전 120ms 유휴 조건을 추가로 검사한다. */
#define COMBO_TERM 40
#define COMBO_STRICT_TIMER
#define COMBO_SHOULD_TRIGGER

/* ── Caps Word ─────────────────────────────────────────────────────────
 * 영문 약어(T2WI · STIR · ACL · DDx) 입력용. 다음 스페이스까지 대문자.
 */
#define CAPS_WORD_IDLE_TIMEOUT 4000

/* ── One-shot (sticky shift) ───────────────────────────────────────────*/
#define ONESHOT_TIMEOUT 2500

/* ── Grave Escape ──────────────────────────────────────────────────────
 * QK_GESC :  Esc  /  Shift+Esc = ~  /  Win+Esc = `
 * AHK 의 `+Esc::Send("~")` 를 대체한다(2026-09-05 폐기).
 */

/* ── 매크로 타이밍 ─────────────────────────────────────────────────────
 * 편집 매크로(단어/행 삭제)가 PACS 편집기에서 씹히면 이 값을 올린다.
 * keymap.c 의 SS_DELAY 도 함께 조정할 것.
 */
#define TAP_CODE_DELAY 10

/* 사용하지 않는 기능 (펌웨어 크기 절약) */
#define NO_ACTION_ONESHOT_DISABLE 0   /* one-shot 은 sticky shift 에 필요 */
