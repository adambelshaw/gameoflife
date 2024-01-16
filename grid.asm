; CONSTANTS
COMPRESSED_GRID_WIDTH: equ $10 ; half of GRID_WIDTH rounded up to next even number (16)
GRID_HEIGHT: equ $17 ; 23
GRID_WIDTH: equ $1f ; 31
MAX_ACTIVE_CELLS: equ $ff ; 255

SECTION code_user

;----------
; get_grid_value
; inputs: d = y, e = x
; outputs: hl = grid value
; alters: af, bc, de, hl
;----------
get_grid_value:
            ld b, d
            ld c, e
            call load_cell_location            
            ld a, (hl) ; load 8 bit value into a
            bit $00, c ; is x even?
            jr z, get_grid_value_even
            or a ; clear carry so doesn't get rotated into number
            rra
            rra
            rra
            rra ; rotate the last 4 bits to the first 4
            jr get_grid_value_end
get_grid_value_even:
            and $0f ; blank out the last 4 bits
get_grid_value_end:
            ld h, $00
            ld l, a ; hl = grid value
            ret

;----------
; set_grid_value_asm
; inputs: b = y, c = x, e = grid value
; alters: a, bc, de, hl
;----------
set_grid_value:
            pop hl ; hl = ret address
            pop bc ; b = y, c = x
            pop de ; e = grid value
            push hl ; ret address back on stack
            ld a, e ; a = grid value
            ex af, af' ; store a
            call load_cell_location ; load cell location bc into hl
            ex af, af' ; retrieve a            
            bit $00, c ; is x even?
            jr z, set_grid_value_even
            or a ; clear carry so doesn't get rotated into number
            rla ; x not even
            rla
            rla
            rla ; rotate the first 4 bits to the last 4
            ld e, a ; e = given value on rhs
            ld a, (hl)            
            and $0f ; a = current lhs value
            jr set_grid_value_end
set_grid_value_even: ; x is even
            and $0f ; blank out the last 4 bits so we don't overwrite
            ld e, a ; e = given value on lhs
            ld a, (hl)            
            and $f0 ; a = current rhs value
set_grid_value_end:
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

;----------
; get_cell_location
; inputs: d = y, e = x
; outputs: hl = cell location within grid = (y * GRID_WIDTH) + x + 1
; alters: a, bc, de, hl
;----------
get_cell_location:
            ld l, $00
            ld h, $00
            ld c, e ; c = x
            ld e, d
            ld d, $00 ; de = y
            ld b, GRID_WIDTH
get_cell_location_loop:
            add hl, de
            djnz get_cell_location_loop
            ld e, c ; de - x
            add hl, de
            add hl, 1 ; hl = (y * GRID_WIDTH) + x + 1
            ret

;----------
; get_cell_x_coord
; inputs: bc = cell location
; outputs: hl = x coord = (cell_location % GRID_WIDTH) - 1
; alters: a, bc, de, hl
;----------
get_cell_x_coord:
            ld d, $00
            ld e, GRID_WIDTH ; de = GRID_WIDTH
            ld hl, $00    ; accumulator
            ld a, $10    ; loop counter
get_cell_x_coord_loop:
            ; shift the bits from bc (numerator) into hl (accumulator)
            sla c \ rl b
            adc hl,hl
            ; check if remainder >= denominator (hl>=de)
            sbc hl,de
            jr c, get_cell_x_coord_loop_readjust
            inc c
            jr get_cell_x_coord_loop_done
get_cell_x_coord_loop_readjust:
            ; remainder is not >= denominator, so we have to add de back to hl
            add hl, de
get_cell_x_coord_loop_done:
            dec a
            jr nz, get_cell_x_coord_loop
            dec hl ; hl = (cell_location % GRID_WIDTH) - 1
            ret

PUBLIC get_cell_y_coord
;----------
; get_cell_y_coord
; inputs: bc = cell location
; outputs: hl = y coord = (cell_location / GRID_WIDTH)
; alters: a, bc, de, hl
;----------
get_cell_y_coord:
            ld d, $00
            ld e, GRID_WIDTH ; de = GRID_WIDTH
            ld hl, $00 ; accumulator
            ld a, $10 ; loop counter
get_cell_y_coord_loop:
            ; shift the bits from bc (numerator) into hl (accumulator)
            sla c \ rl b
            adc hl, hl
            ; check if remainder >= denominator (hl>=de)
            sbc hl, de
            jr c, get_cell_y_coord_loop_readjust
            inc c
            jr get_cell_y_coord_loop_done
get_cell_y_coord_loop_readjust:
            ; remainder is not >= denominator, so we have to add de back to hl
            add hl, de
get_cell_y_coord_loop_done:
            dec a
            jr nz, get_cell_y_coord_loop
            ld h, b
            ld l, c ; hl = cell_location / GRID_WIDTH
            ret

SECTION data_user
grid: ds COMPRESSED_GRID_WIDTH*GRID_HEIGHT
updated_cell_count: ds 1, $00 ; extern uint8_t updated_cell_count;
updated_cells: ds MAX_ACTIVE_CELLS*2, $00 ; extern uint16_t updated_cells[];
active_cell_count: ds 1, $00 ; extern uint8_t active_cell_count;
active_cells: ds MAX_ACTIVE_CELLS*2, $00 ; extern uint16_t active_cells[];