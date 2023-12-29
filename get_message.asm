; CONSTANTS
KEYDEL: equ $0c            ; ASCII value for delete
KEYENT: equ $0d            ; ASCII value for enter
KEYSPC: equ $20            ; ASCII value for space
FLAGS_KEY: equ $5c3b       ; ROM address for keypad status when using im1
LAST_KEY: equ $5c08        ; ROM address for last key pressed when using im1
CURSOR: equ $5c88          ; ROM address for cursor position on screen channel 1 - if loaded at bc then b = y, c = x
LOCATE: equ $0dd9          ; ROM address for AT routine to position the cursor
MAX_MSG_LENGTH: equ $32    ; the maximum message length is 50

SECTION code_user

PUBLIC _get_message
;----------
; _get_message
; prompts user and populates _message
; alters: af, bc, de, hl
;----------
_get_message:
        ld   hl, message_prompt
        call print_string        
        ld   hl, _message   ; hl = address of message
        ld   (hl), $00        
        ld   d, $00         ; d = tracks the mssage length
        ei                  ; enable interrupts (mode 1) so we can use ROM input routines
        call get_message_loop
        di                  ; disable interrupts again
        ret
get_message_loop:
        push hl             ; preserve hl
        call wait_key       ; wait for valid key
        pop  hl             ; retrieve hl
        cp   KEYDEL         ; delete?
        jr   z, get_message_delete ; yes, goto delete
        cp   KEYENT         ; enter?
        jr   z, get_message_enter ; yes, goto enter
        push de             ; preserves de
        ld   e, a           ; e = code ASCII
        ld   a, MAX_MSG_LENGTH  ; a = maximum message length
        cp   d              ; d = maximum length?
        ld   a, e           ; a = code ASCII
        pop  de             ; retrieve de
        jr   z, get_message_loop ; d = maximum length so loop without appending
        ld   (hl), a        ; append character to name
        inc  hl             ; hl = next position
        rst  $10            ; print character
        inc  d              ; increment message length counter
        jr   get_message_loop ; wait for next character
get_message_delete:
        ld   a, $00         ; a = 0
        cp   d              ; length 0?
        jr   z, get_message_loop ; yes, wait for next character
        dec  d              ; decrement message length counter
        dec  hl             ; hl-=1, previous character
        ld   a, ' '         ; a = space
        ld   (hl), a        ; clear previous character
        ld   bc, (CURSOR)   ; bc = cursor position
        inc  c              ; bc = previous column for AT
        call AT             ; position cursor
        rst  $10            ; delete the display character	
        call AT             ; position cursor
        jr   get_message_loop ; wait for next character
get_message_enter:
        ld   a, 0           ; a = 0
        cp   d              ; length 0?
        jr   z, get_message_loop ; yes, request another character
        ret                 ; exit with populated message

;----------
; print_string
; inputs: hl = first position of a null ($00) terminated string
; alters: af, hl
;----------
print_string:
        ld   a, (hl)        ; a = character to be printed
        or   a              ; sets z register if 0
        ret  z              ; return if z register set
        rst  $10            ; prints the character
        inc  hl             ; hl = next character
        jr   print_string   ; loop

;----------
; wait_key
; outputs: a = ASCII code of the next pressed key
; alters: af, hl
;----------
wait_key:
        ld   hl, FLAGS_KEY         ; hl = address flag keyboard
        set  $03, (hl)             ; input mode L
wait_key_loop:
        bit  $05, (hl)             ; key pressed?
        jr   z, wait_key_loop      ; not pressed, loop
        res  $05, (hl)             ; bit set to 0 for future inspections
wait_key_load_key:
        ld   hl, LAST_KEY          ; hl = last key pressed address
        ld   a, (hl)               ; a = last key pressed
        cp   $80                   ; ASCII > 127?
        jr   nc, wait_key          ; yes, invalid key, loop
        cp   KEYDEL                ; delete?
        ret  z                     ; yes, exit
        cp   KEYENT                ; enter?
        ret  z                     ; yes, exit
        cp   KEYSPC                ; space?
        jr   c, wait_key           ; ASCII < space, invalid, loop
        ret                        ; exits

;----------
; AT
; position the cursor while maintaining registers - the upper corner is at 24, 33
; inputs: b = y, c = x
;----------
AT:
        push af
        push bc
        push de
        push hl                    ; preserve registers
        call LOCATE                ; position cursor using ROM routine
        pop  hl
        pop  de
        pop  bc
        pop  af                    ; retrieves records
        ret

SECTION data_user
PUBLIC _message
_message: ds MAX_MSG_LENGTH+1

SECTION rodata_user
message_prompt:
db "Welcome To ZX Life!", KEYENT, KEYENT, "Please enter message to display,maximum 50 characters:", KEYENT, KEYENT, $00