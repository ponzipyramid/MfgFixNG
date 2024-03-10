Scriptname MfgConsoleFunc Hidden

; native functions, wrapper base. these have magic number for select some parameter.
bool function SetPhonemeModifier(Actor act, int mode, int id, int value) native global
int function GetPhonemeModifier(Actor act, int mode, int id) native global

;API function for applying expression preset on actor
;        =Arguments=
;akActor            = actor to process
;apExpression       = raw expression construct.
;aiStrength         = strength which will be used to modify expression. Range 1-100
;abOpenMouth        = if true, will edit phonems to open mouth little
;exprPower          = used if expression is set but expr value is 0 in the preset (dynamic expr) 
;exprStrModifier, modStrModifier,  phStrModifier= values will be miltiplied by those
;afSpeed            = impacts amount of iterations
;aiDelay            = timemout in milliseconds between each iteration
;        =Return value=
;Return true in case that expression was successfully applied
; Apply expression array [31] to actor akActor
bool Function ApplyExpressionPreset(Actor akActor, float[] aaExpression, bool abOpenMouth, int exprPower, float exprStrModifier, float modStrModifier, float phStrModifier)  global native 
;aiMode -1 for reset all, 0 for phonemes, 1 for modifiers
bool function ResetMFGSmooth(Actor akActor, int aiMode) global native

; wrapper functions

; set phoneme/modifier, same as console.
bool function SetPhoneme(Actor act, int id, int value) global
	return SetPhonemeModifier(act, 0, id, value)
endfunction
bool function SetModifier(Actor act, int id, int value) global
	return SetPhonemeModifier(act, 1, id, value)
endfunction
bool function SetExpression(Actor act, int mood, int value) global
	return SetPhonemeModifier(act, 2, mood, value)
endfunction
; reset phoneme/modifier. this does not reset expression.
bool function ResetPhonemeModifier(Actor act) global
	return SetPhonemeModifier(act, -1, 0, 0)
endfunction

; get phoneme/modifier/expression
int function GetPhoneme(Actor act, int id) global
	return GetPhonemeModifier(act, 0, id)
endfunction
int function GetModifier(Actor act, int id) global
	return GetPhonemeModifier(act, 1, id)
endfunction

; return expression value which is enabled. (enabled only one at a time.)
int function GetExpressionValue(Actor act) global
	return GetPhonemeModifier(act, 2, 0)
endfunction
; return expression ID which is enabled.
int function GetExpressionID(Actor act) global
	return GetPhonemeModifier(act, 3, 0)
endfunction

