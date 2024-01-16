VIDEORAM: equ $4000 ; address of video RAM
VIDEORAM_L: equ $1800 ; length of video RAM
VIDEOATT: equ $5800 ; address of attribute RAM
VIDEOATT_L: equ $0300 ; length of attribute RAM

SECTION code_user

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

;----------
; clear_cell_at
; inputs: b = y, c = x
; alters: a, bc, de
;----------
clear_cell_at:
            pop hl ; hl = ret address
            pop de ; d = y, e = x
            push hl ; ret address back on stack

            call get_attr_address
            ld (hl), @00111000 ; paper white

            ex de, hl ; h = y, l = x
            call get_char_address
            ld de, clear_sprite ; h = y, l = x, de = address of glyph
            call print_char_at
            ret

;----------
; print_cell_at
; inputs: d = y, e = x, c = ink
; alters: a, bc, de, hl
;----------
print_cell_at:
            pop hl ; hl = ret address
            pop de ; d = y, e = x
            pop bc ; c = ink
            push hl ; ret address back on stack

            call get_attr_address
            ld a, c ; a = ink
            or @00111000 ; paper white
            ld (hl), a ; set attribute value

            ex de, hl ; h = y, l = x
            call get_char_address
            ld de, cell_sprite ; h = y, l = x, de = address of glyph
            call print_char_at
            ret

;----------
; print_char_at
; inputs: h = y, l = x, de = location of char
; alters: a, bc, de, hl
;----------
print_char_at:
            ld b, 8 ; loop counter
print_char_at_loop:
            ld a, (de) ; get the byte
            ld (hl), a ; print to screen
            inc de ; goto next byte of character
            inc h ; goto next line of screen
            djnz print_char_at_loop ; loop 8 times
            ret

;----------
; print_block_at
; inputs: d = y, e = x, c = ink
; alters: a, bc, de
;----------
print_block_at:
            pop hl ; hl = ret address
            pop de ; d = y, e = x
            pop bc ; c = ink
            push hl ; ret address back on stack
            
            call get_attr_address
            ld a, c ; a = ink
            or @01111000 ; paper white, bright
            ld (hl), a ; set attribute value
            
            ex de, hl ; h = y, l = x
            call get_char_address
            ld de, block_sprite ; h = y, l = x, de = address of glyph
            call print_char_at
            ret

;----------
; get_char_address
; inputs: h = y, l = x
; outputs: hl = location of screen address
; alters: hl
;----------
get_char_address:
            ld a,h
            and $07
            rra
            rra
            rra
            rra
            or l
            ld l,a
            ld a,h
            and $18
            or $40
            ld h,a
            ret	

;----------
; get_attr_address
; inputs: d = y, e = x
; outputs: hl = location of attribute address
; alters: hl
;----------
get_attr_address:
            ld a,d
            rrca
            rrca
            rrca
            ld l,a
            and $03
            add a, $58
            ld h,a
            ld a,l
            and $E0
            ld l,a
            ld a,e
            add a,l
            ld l,a
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

block_sprite:
            defb @11111111
            defb @11111111
            defb @11111111
            defb @11111111
            defb @11111111
            defb @11111111
            defb @11111111
            defb @11111111

clear_sprite:
            defb @00000000
            defb @00000000
            defb @00000000
            defb @00000000    
            defb @00000000
            defb @00000000
            defb @00000000
            defb @00000000