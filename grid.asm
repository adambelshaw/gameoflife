; CONSTANTS
COMPRESSED_GRID_WIDTH: equ $10 ; half of GRID_WIDTH rounded up to next even number (16)
GRID_HEIGHT: equ $17 ; 23
GRID_WIDTH: equ $1f ; 31
MAX_ACTIVE_CELLS: equ $ff ; 255

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
            or a ; clear carry so doesn't get rotated into number
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
            or a ; clear carry so doesn't get rotated into number
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
            or a ; clear carry flag so doesn't get rotated into number
            rra ; shift-right i.e. divide by 2
            ld hl, grid ; point hl at grid
            ld d, $00 
            ld e, a ; de = a
            add hl, de ; hl = _grid + x/2
            ;ld d, $00 - already 0
            ld e, b ; de = y           
            ld b, COMPRESSED_GRID_WIDTH ; load b to loop COMPRESSED_GRID_WIDTH times
load_cell_location_loop: 
            add hl, de
            djnz load_cell_location_loop ; hl = _grid + (x / 2) + (y * COMPRESSED_GRID_WIDTH)
            ret

PUBLIC _get_cell_location_asm
;----------
; get_cell_location_asm
; inputs: d = y, e = x
; outputs: hl = cell location within grid = (y * GRID_WIDTH) + x + 1
; alters: a, bc, de, hl
;----------
_get_cell_location_asm:
            ; extern uint16_t get_cell_location_asm(uint8_t x, uint8_t y) __z88dk_callee;
            pop hl ; hl = ret address
            pop de ; d = y, e = x
            push hl ; ret address back on stack
            ld l, $00
            ld h, $00
            ld c, e ; c = x
            ld e, d
            ld d, $00 ; de = y
            ld b, GRID_WIDTH
_get_cell_location_asm_loop:
            add hl, de
            djnz _get_cell_location_asm_loop
            ld e, c ; de - x
            add hl, de
            add hl, 1 ; hl = (y * GRID_WIDTH) + x + 1
            ret

PUBLIC _get_cell_x_coord_asm
;----------
; get_cell_x_coord_asm
; inputs: bc = cell location
; outputs: hl = x coord = (cell_location % GRID_WIDTH) - 1
; alters: a, bc, de, hl
;----------
_get_cell_x_coord_asm:
            ; extern uint8_t get_cell_x_coord_asm(uint16_t cell_location) __z88dk_callee;
            pop hl ; hl = ret address
            pop bc ; bc = cell location
            push hl ; ret address back on stack
            ld d, $00
            ld e, GRID_WIDTH ; de = GRID_WIDTH
            ld hl, $00    ; accumulator
            ld a, $10    ; loop counter
_get_cell_x_coord_asm_loop:
            ; shift the bits from bc (numerator) into hl (accumulator)
            sla c \ rl b
            adc hl,hl
            ; check if remainder >= denominator (hl>=de)
            sbc hl,de
            jr c,_get_cell_x_coord_asm_loop_readjust
            inc c
            jr _get_cell_x_coord_asm_loop_done
_get_cell_x_coord_asm_loop_readjust:
            ; remainder is not >= denominator, so we have to add de back to hl
            add hl, de
_get_cell_x_coord_asm_loop_done:
            dec a
            jr nz, _get_cell_x_coord_asm_loop
            dec hl ; hl = (cell_location % GRID_WIDTH) - 1
            ret

PUBLIC _get_cell_y_coord_asm
;----------
; get_cell_y_coord_asm
; inputs: bc = cell location
; outputs: hl = y coord = (cell_location / GRID_WIDTH)
; alters: a, bc, de, hl
;----------
_get_cell_y_coord_asm:
            ; extern uint8_t get_cell_x_coord_asm(uint16_t cell_location) __z88dk_callee;
            pop hl ; hl = ret address
            pop bc ; bc = cell location
            push hl ; ret address back on stack
            ld d, $00
            ld e, GRID_WIDTH ; de = GRID_WIDTH
            ld hl, $00 ; accumulator
            ld a, $10 ; loop counter
_get_cell_y_coord_asm_loop:
            ; shift the bits from bc (numerator) into hl (accumulator)
            sla c \ rl b
            adc hl, hl
            ; check if remainder >= denominator (hl>=de)
            sbc hl, de
            jr c, _get_cell_y_coord_asm_loop_readjust
            inc c
            jr _get_cell_y_coord_asm_loop_done
_get_cell_y_coord_asm_loop_readjust:
            ; remainder is not >= denominator, so we have to add de back to hl
            add hl, de
_get_cell_y_coord_asm_loop_done:
            dec a
            jr nz, _get_cell_y_coord_asm_loop
            ld h, b
            ld l, c ; hl = cell_location / GRID_WIDTH
            ret

SECTION data_user
grid: ds COMPRESSED_GRID_WIDTH*GRID_HEIGHT
updated_cell_count: ds 1, $00 ; extern uint8_t updated_cell_count;
updated_cells: ds MAX_ACTIVE_CELLS*2, $00 ; extern uint16_t updated_cells[];
active_cell_count: ds 1, $00 ; extern uint8_t active_cell_count;
active_cells: ds MAX_ACTIVE_CELLS*2, $00 ; extern uint16_t active_cells[];