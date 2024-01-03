; CONSTANTS
COMPRESSED_GRID_WIDTH: equ $10    ; half of GRID_WIDTH rounded up to next even number (16)
GRID_HEIGHT: equ $17              ; 23

SECTION code_user

PUBLIC _get_grid_value_asm
;----------
; get_grid_value_asm
; inputs: b = y, c = x
; outputs: hl = grid value
; alters: a, bc, de, hl
;----------
_get_grid_value_asm:
            ; extern uint8_t get_grid_value_asm(uint8_t x, uint8_t y) __z88dk_callee;
            pop hl ; hl = ret address
            pop bc ; b = y, c = x
            push hl ; ret address back on stack

            call load_cell_location
            ld a, (hl) ; load 8 bit value into a
            bit $00, c ; is x even?
            jr z, _get_grid_value_asm_even
            rra
            rra
            rra
            rra ; rotate the last 4 bits to the first 4
            jr _get_grid_value_asm_end
_get_grid_value_asm_even:
            and $0f ; blank out the last 4 bits
_get_grid_value_asm_end:
            ld h, $00
            ld l, a ; hl = grid value
            ret

PUBLIC _set_grid_value_asm
;----------
; set_grid_value_asm
; inputs: b = y, c = x, e = grid value
; alters: a, bc, de, hl
;----------
_set_grid_value_asm:
            ; extern void set_grid_value_asm(uint8_t x, uint8_t y, uint16_t value) __z88dk_callee; // last parameter uint16_t as crashes if odd number of 8 bits
            pop hl ; hl = ret address
            pop bc ; b = y, c = x
            pop de ; e = grid value
            push hl ; ret address back on stack

            ld a, e ; a = grid value
            ex af, af' ; store a
            call load_cell_location ; load cell location bc into hl
            ex af, af' ; retrieve a            
            bit $00, c ; is x even?
            jr z, _set_grid_value_asm_even
            rla ; x not even
            rla
            rla
            rla ; rotate the first 4 bits to the last 4
            ld e, a ; e = given value on rhs
            ld a, (hl)            
            and $0f ; a = current lhs value
            jr _set_grid_value_asm_end
_set_grid_value_asm_even: ; x is even
            and $0f ; blank out the last 4 bits so we don't overwrite
            ld e, a ; e = given value on lhs
            ld a, (hl)            
            and $f0 ; a = current rhs value
_set_grid_value_asm_end:
            or e ; a = combined given and current value
            ld (hl), a ; store back in location
            ret

;----------
; load_cell_location
; inputs: b = y, c = x
; outputs: hl = cell location within grid
; alters: a, b, de, hl
;----------
load_cell_location:
            ld a, c ; load a with x
            rra     ; shift-right i.e. divide by 2

            ld hl, grid ; point hl at grid
            ld d, $00 
            ld e, a ; de = a
            add hl, de ; hl = _grid + x/2
            ;ld d, $00 - already 0
            ld e, b ; de = y           
            ld b, COMPRESSED_GRID_WIDTH ; load b to loop COMPRESSED_GRID_WIDTH times
load_cell_location_loop: 
            add hl, de
            djnz load_cell_location_loop            
            ; hl = _grid + (x / 2) + (y * COMPRESSED_GRID_WIDTH)
            ret

SECTION data_user
grid: ds COMPRESSED_GRID_WIDTH*GRID_HEIGHT