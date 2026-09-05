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

/* 같은 손 롤로 인한 mod-tap 오발동 억제.
 * ⚠ Chordal Hold 는 "반대손 조합만 hold" 규칙이다. FN 이 왼엄지고 화살표도 왼손이라
 *   그대로 두면 레이어가 tap 으로 settle 되어 동작하지 않는다.
 *   keymap.c 의 chordal_hold_layout 에서 엄지·row3 여분을 '*' 로 면제했다.
 *   https://docs.qmk.fm/tap_hold#chordal-hold */
#define CHORDAL_HOLD

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
