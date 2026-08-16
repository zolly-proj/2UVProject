import keyboard

def key_stroke():
	pause_keyboard = False
	while True:
		pause_keyboard = True
		if keyboard.is_pressed('c'):
			pause_keyboard = False
			break
		if pause_keyboard:
			continue

key_stroke()
