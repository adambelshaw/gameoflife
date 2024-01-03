; CONSTANTS
UDG: equ $5c7b ; RAM address of user defined graphics
VIDEORAM: equ $4000 ; address of video RAM
VIDEORAM_L: equ $1800 ; length of video RAM
VIDEOATT: equ $5800 ; address of attribute RAM
VIDEOATT_L: equ $0300 ; length of attribute RAM
LOCATE: equ $0dd9 ; ROM address for AT routine to position the cursor

SECTION code_user

PUBLIC _load_graphics_asm
;----------
; load_graphics_asm
; alters: hl
;----------
_load_graphics_asm:
            ld hl, cell_sprite ; load cell sprite location
            ld (UDG), hl ; load as first UDG
            ret

PUBLIC _print_string_asm
;----------
; print_string_asm
; inputs: hl = first position of a null ($00) terminated string
; alters: af, hl
;----------
_print_string_asm:
            ld   a, (hl) ; a = character to be printed
            or   a ; sets z register if 0
            ret  z ; return if z register set
            rst  $10 ; prints the character
            inc  hl ; hl = next character
            jr   _print_string_asm ; loop

PUBLIC _clear_screen_asm
;----------
; clear_screen_asm
; clears all pixels, sets ink to black and paper white
; alters: bc, de, hl
;----------
_clear_screen_asm:
            ; clear pixels
            ld hl, VIDEORAM ; hl = video RAM address
            ld de, VIDEORAM+1 ; de = next address
            ld bc, VIDEORAM_L-1 ; bc = length of video RAM - 1 (to loop)
            ld (hl), $00 ; clear first position
            ldir ; loop and clear the rest
            ; clear attributes
            ld hl, VIDEOATT ; hl = attribute RAM address
            ld de, VIDEOATT+1 ; de = next address
            ld bc, VIDEOATT_L-1 ; bc = length of attribute RAM - 1 (to loop)
            ld (hl), @00111000 ; paper white, ink black
            ldir ; loop and set the rest            
            ret

PUBLIC _clear_cell_at_asm
;----------
; clear_cell_at_asm
; inputs: b = y, c = x
; alters: a, bc, de
;----------
_clear_cell_at_asm:
            ; extern void clear_cell_at_asm(uint8_t x, uint8_t y) __z88dk_callee;
            pop hl ; hl = ret address
            pop de ; d = y, e = x
            push hl ; ret address back on stack
            call convert_x_y_coords
            call LOCATE ; call LOCATE ROM routine
            ld a, $13 ; control code for set bright
            rst $10 ; call PRINT ROM routine
            ld a, 0 ; bright value
            rst $10
            ld a, ' ' ; clear
            rst $10
            ret

PUBLIC _print_cell_at_asm
;----------
; print_cell_at_asm
; inputs: d = y, e = x, c = ink
; alters: a, bc, de, hl
;----------
_print_cell_at_asm:
            ; extern void print_cell_at_asm(uint8_t x, uint8_t y, uint16_t ink) __z88dk_callee;
            pop hl ; hl = ret address
            pop de ; d = y, e = x
            pop bc ; c = ink
            push hl ; ret address back on stack
            push bc ; store bc (ink)
            call convert_x_y_coords
            call LOCATE ; call LOCATE ROM routine
            pop bc ; retrieve bc (ink)
            ld a, $10 ; control code for set ink
            rst $10 ; call PRINT ROM routine
            ld a, c ; ink value            
            rst $10
            ld a, $13 ; control code for set bright
            rst $10
            ld a, 0 ; bright value
            rst $10
            ld a, $90 ; cell UDG stored at $90
            rst $10 ; print
            ret

PUBLIC _print_block_at_asm
;----------
; print_block_at_asm
; inputs: d = y, e = x, c = ink
; alters: a, bc, de
;----------
_print_block_at_asm:
            ; extern void print_block_at_asm(uint8_t x, uint8_t y, uint16_t ink) __z88dk_callee;
            pop hl ; hl = ret address
            pop de ; d = y, e = x
            pop bc ; c = ink
            push hl ; ret address back on stack
            push bc ; store bc (ink)
            call convert_x_y_coords            
            call LOCATE ; call LOCATE ROM routine
            pop bc ; retrieve bc (ink)
            ld a, $10 ; control code for set ink
            rst $10 ; call PRINT ROM routine
            ld a, c ; ink value            
            rst $10
            ld a, $13 ; control code for set bright
            rst $10
            ld a, 1 ; bright value
            rst $10
            ld a, $8F ; print block
            rst $10
            ret

;----------
; convert_x_y_coords
; inputs: d = y, e = x (top left is 0,0)
; outputs: b = y, c = x (top left is 24,33 - as expected by ROM AT)
; alters: a, bc, de
;----------
convert_x_y_coords:
            ld a, $18
            ld b, d
convert_x_y_coords_y_loop:
            dec a
            djnz convert_x_y_coords_y_loop  
            ld d, a ; y = $18-y
            ld a, $21
            ld b, e
convert_x_y_coords_x_loop:
            dec a
            djnz convert_x_y_coords_x_loop  
            ld e, a ; x = $21-x
            ld b, d ; b = y
            ld c, e ; c = x
            ret

PUBLIC _get_glyph_from_chr_asm
;----------
; get_glyph_from_chr_asm
; inputs: l = character
; outputs: hl = pointer to glyph
; alters: hl
;----------
_get_glyph_from_chr_asm:
            ld h, $00 ; make sure h is 00
            add hl, hl
            add hl, hl
            add hl, hl ; h *= 8
            add hl, $3C00 ; add start of character fonts
            ret

SECTION rodata_user

cell_sprite:
	defb @10101010
	defb @01010101
	defb @10101010
	defb @01010101
	defb @10101010
	defb @01010101
	defb @10101010
	defb @01010101