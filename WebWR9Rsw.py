import ctypes
import webview

def setup():
    # Create the window with specific dimensions
    webview.create_window('WR9R Ant Selector', 'http://192.168.1.179', width=300, height=450)

if __name__ == '__main__':
    # Minimize the console window
    ctypes.windll.user32.ShowWindow(ctypes.windll.kernel32.GetConsoleWindow(), 6)
    
    # Call the setup function before starting
    setup()
    webview.start()
