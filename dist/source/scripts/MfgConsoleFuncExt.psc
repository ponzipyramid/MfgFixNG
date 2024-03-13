Scriptname MfgConsoleFuncExt Hidden

;Applying expression array [31] to actor akActor
;        =Arguments=
;akActor            = actor to process
;apExpression       = raw expression construct.
;abOpenMouth        = if true, will edit phonems to open mouth little
;exprPower          = used if expression is set but expr value is 0 in the preset (dynamic expr strength). 0 to ignore
;exprStrModifier, modStrModifier,  phStrModifier= values will be miltiplied by those (capped by 200)
;speed              = anim speed. 0.1 is close to instant. 0.75 is recomended for smooth transitions 
;        =Return value=
;Return true in case preset was successfully applied
bool Function ApplyExpressionPreset(Actor akActor, float[] aaExpression, bool abOpenMouth, int exprPower, float exprStrModifier, float modStrModifier, float phStrModifier, float speed)  native global

;Reset mfg smoothly
;        =Arguments=
;akActor            = actor to process
;mode      		=  -1 for reset all, 0 for phonemes, 1 for modifiers
;speed              = anim speed. 0.1 is close to instant. 0.75 is recomended for smooth transitions 
;        =Return value=
;Return true if successfully applied
bool function ResetMFGSmooth(Actor akActor, int mode, float speed) native global

;Set mfg smoothly
;        =Arguments=
;akActor            = actor to process
;mode        		=  Reset= -1,Phoneme= 0,Modifier= 1,ExpressionValue = 2,
;id                 = phoneme or modifier or mood id
;value              = phoneme or modifier or expression strength
;speed              = anim speed. 0.1 is close to instant. 0.75 is recomended for smooth transitions 
;        =Return value=
;Return true if successfully applied
bool function SetPhonemeModifierSmooth(Actor act, int mode, int id, int value, float speed) native global

; Get PC dialogue target
Actor Function GetPlayerSpeechTarget() global native

; wrapper functions

; set phoneme/modifier, same as console.
bool function SetPhoneme(Actor act, int id, int value, float speed = 0.75) global
	return SetPhonemeModifierSmooth(act, 0, id, value, speed)
endfunction
bool function SetModifier(Actor act, int id, int value, float speed = 0.75) global
	return SetPhonemeModifierSmooth(act, 1, id, value, speed)
endfunction
bool function SetExpression(Actor act, int mood, int value, float speed = 0.75) global
	return SetPhonemeModifierSmooth(act, 2, mood, value, speed)
endfunction
bool Function ApplyExpressionPresetSmooth(Actor akActor, float[] aaExpression, bool abOpenMouth, int exprPower=0, float exprStrModifier=1.0, float modStrModifier=1.0, float phStrModifier=1.0, float speed=0.75) global
	return ApplyExpressionPreset(akActor,aaExpression,abOpenMouth,exprPower,exprStrModifier,modStrModifier,phStrModifier, speed)
endfunction
bool function ResetMfg(Actor act, float speed = 0.75) global
	return ResetMFGSmooth(act,-1, speed)
endfunction
bool function ResetPhonemes(Actor act, float speed = 0.75) global
	return ResetMFGSmooth(act, 0, speed)
endfunction
bool function ResetModifiers(Actor act, float speed = 0.75) global
	return ResetMFGSmooth(act, 1, speed)
endfunction