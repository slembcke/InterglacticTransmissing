;this file for FamiTone2 library generated by text2data tool

Space_Radar_2_1_0__1__music_data:
	.byte 1
	.word @instruments
	.word @samples-3
	.word @song0ch0,@song0ch1,@song0ch2,@song0ch3,@song0ch4,307,256

@instruments:
	.byte $b0 ;instrument $00
	.word @env1,@env0,@env0
	.byte $00
	.byte $30 ;instrument $01
	.word @env1,@env0,@env0
	.byte $00
	.byte $30 ;instrument $02
	.word @env1,@env0,@env0
	.byte $00
	.byte $b0 ;instrument $03
	.word @env1,@env0,@env0
	.byte $00

@samples:
@env0:
	.byte $c0,$00,$00
@env1:
	.byte $cf,$00,$00


@song0ch0:
	.byte $fb,$05
@song0ch0loop:
@ref0:
	.byte $00,$f9,$c3
@ref1:
	.byte $00,$f9,$c3
@ref2:
	.byte $00,$f9,$c3
@ref3:
	.byte $00,$f9,$c3
@ref4:
	.byte $00,$f9,$c3
@ref5:
	.byte $80,$5b,$55,$5b,$54,$99,$5b,$55,$5b,$55,$5b,$54,$99,$5b,$55,$5f
	.byte $55,$5f,$54,$99,$5f,$55,$63,$59,$63,$59,$63,$59,$53,$4b,$53,$4b
	.byte $53,$58,$81
@ref6:
	.byte $5b,$55,$5b,$54,$99,$5b,$55,$5b,$55,$5b,$54,$99,$5b,$55,$5f,$55
	.byte $5f,$54,$99,$5f,$55,$63,$59,$63,$59,$63,$59,$53,$4b,$53,$4b,$53
	.byte $58,$81
	.byte $ff,$22
	.word @ref6
	.byte $ff,$22
	.word @ref6
	.byte $fd
	.word @song0ch0loop

@song0ch1:
@song0ch1loop:
@ref9:
	.byte $00,$f9,$c3
@ref10:
	.byte $00,$89,$82,$24,$95,$28,$89,$2a,$a1,$32,$89,$2e,$95,$24,$ad,$22
	.byte $95
@ref11:
	.byte $00,$89,$24,$95,$28,$89,$2a,$95,$32,$95,$36,$a1,$2e,$89,$3a,$ad
@ref12:
	.byte $00,$89,$24,$95,$28,$89,$2a,$a1,$32,$89,$2e,$95,$24,$ad,$22,$95
	.byte $ff,$10
	.word @ref11
@ref14:
	.byte $00,$89,$24,$95,$28,$89,$2a,$a1,$32,$85,$2b,$2e,$95,$24,$95,$22
	.byte $91,$19,$1a,$95
@ref15:
	.byte $00,$89,$24,$95,$28,$89,$2a,$95,$32,$91,$35,$36,$9d,$2d,$2e,$85
	.byte $39,$3a,$89,$2d,$2f,$31,$32,$95
	.byte $ff,$14
	.word @ref14
	.byte $ff,$18
	.word @ref15
	.byte $fd
	.word @song0ch1loop

@song0ch2:
@song0ch2loop:
@ref18:
	.byte $00,$f9,$c3
@ref19:
	.byte $84,$25,$00,$85,$25,$00,$85,$25,$00,$85,$24,$85,$29,$2b,$00,$85
	.byte $2b,$00,$85,$2b,$00,$85,$2a,$85,$33,$2f,$00,$85,$2f,$00,$85,$2f
	.byte $00,$85,$34,$85,$2f,$33,$00,$85,$2f,$00,$85,$2b,$2f,$2b,$29,$2b
	.byte $28,$81
@ref20:
	.byte $82,$25,$00,$85,$25,$00,$85,$25,$00,$85,$24,$85,$29,$2b,$00,$85
	.byte $2b,$00,$85,$2b,$00,$85,$2a,$85,$33,$2f,$00,$85,$2f,$00,$85,$2f
	.byte $00,$85,$36,$85,$2f,$32,$85,$01,$3a,$85,$01,$40,$85,$01,$46,$85
	.byte $00,$81
	.byte $ff,$31
	.word @ref19
	.byte $ff,$31
	.word @ref20
	.byte $ff,$31
	.word @ref19
	.byte $ff,$31
	.word @ref20
	.byte $ff,$31
	.word @ref19
	.byte $ff,$31
	.word @ref20
	.byte $fd
	.word @song0ch2loop

@song0ch3:
@song0ch3loop:
@ref27:
	.byte $86,$02,$00,$87,$02,$00,$87,$02,$00,$87,$02,$00,$02,$00,$02,$00
	.byte $02,$00,$87,$02,$00,$87,$02,$00,$87,$02,$00,$02,$00,$02,$00,$02
	.byte $00,$93,$02,$00,$93,$02,$00,$87,$02,$00,$83,$02,$00,$02,$00,$83
	.byte $02,$00,$02,$00,$02,$00,$02,$00
@ref28:
	.byte $02,$00,$82,$1c,$00,$1c,$00,$86,$02,$00,$82,$1c,$00,$1c,$00,$86
	.byte $02,$00,$82,$1c,$00,$1c,$00,$86,$02,$00,$02,$00,$02,$00,$02,$00
	.byte $82,$1c,$00,$1c,$00,$86,$02,$00,$82,$1c,$00,$1c,$00,$86,$02,$00
	.byte $82,$1c,$00,$1c,$00,$86,$02,$00,$02,$00,$02,$00,$02,$00,$82,$1c
	.byte $00,$1c,$00,$83,$1c,$00,$1c,$00,$86,$02,$00,$82,$1c,$00,$1c,$00
	.byte $83,$1c,$00,$1c,$00,$86,$02,$00,$82,$1c,$00,$1c,$00,$86,$02,$00
	.byte $82,$1c,$00,$86,$02,$00,$02,$00,$82,$1c,$00,$86,$02,$00,$02,$00
	.byte $02,$00,$02,$00
	.byte $ff,$5e
	.word @ref28
	.byte $ff,$5e
	.word @ref28
	.byte $ff,$5e
	.word @ref28
	.byte $ff,$5e
	.word @ref28
	.byte $ff,$5e
	.word @ref28
	.byte $ff,$5e
	.word @ref28
	.byte $ff,$5e
	.word @ref28
	.byte $fd
	.word @song0ch3loop

@song0ch4:
@song0ch4loop:
@ref36:
	.byte $00,$f9,$c3
@ref37:
	.byte $00,$f9,$c3
@ref38:
	.byte $00,$f9,$c3
@ref39:
	.byte $00,$f9,$c3
@ref40:
	.byte $00,$f9,$c3
@ref41:
	.byte $00,$f9,$c3
@ref42:
	.byte $00,$f9,$c3
@ref43:
	.byte $00,$f9,$c3
@ref44:
	.byte $00,$f9,$c3
	.byte $fd
	.word @song0ch4loop
