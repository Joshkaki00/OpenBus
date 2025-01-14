import pygame
import json
import zmq

class ZeroMQClient:
    """Handles communication with the JUCE backend via ZeroMQ."""
    def __init__(self):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)
        self.socket.connect("tcp://localhost:5555")

    def send_command(self, command):
        """Send JSON commands to JUCE and receive the response."""
        try:
            self.socket.send_json(command)
            reply = self.socket.recv_json()
            print("Reply from JUCE:", reply)
            return reply
        except Exception as e:
            print("Error communicating with JUCE:", e)
            return {"status": "error", "message": str(e)}


class AudioRouterApp:
    """Main application logic."""
    def __init__(self):
        pygame.init()
        self.screen = pygame.display.set_mode((600, 400))
        pygame.display.set_caption("Audio Mixer - Device Management")
        self.font = pygame.font.Font(None, 36)
        self.running = True
        self.client = ZeroMQClient()
        self.inputs = []
        self.outputs = []
        self.selected_input = None
        self.selected_output = None
        self.presets = {}

        # Load devices
        self.fetch_device_list()

    def fetch_device_list(self):
        """Fetch available input and output devices from JUCE backend."""
        command = {"action": "get_device_list"}
        reply = self.client.send_command(command)
        self.inputs = reply.get("inputs", [])
        self.outputs = reply.get("outputs", [])

    def save_preset(self, filename):
        """Save current selections as a preset."""
        preset = {"input": self.selected_input, "output": self.selected_output}
        with open(filename, 'w') as f:
            json.dump(preset, f)

    def load_preset(self, filename):
        """Load a preset and set devices."""
        try:
            with open(filename, 'r') as f:
                preset = json.load(f)
                self.selected_input = preset.get("input")
                self.selected_output = preset.get("output")
        except FileNotFoundError:
            print("Preset file not found.")

    def render_text(self, text, x, y):
        """Render text on the screen."""
        label = self.font.render(text, True, (255, 255, 255))
        self.screen.blit(label, (x, y))

    def handle_event(self, event):
        """Handle user input."""
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_1:
                self.selected_input = self.inputs[0] if self.inputs else None
            elif event.key == pygame.K_2:
                self.selected_output = self.outputs[0] if self.outputs else None
            elif event.key == pygame.K_s:
                self.save_preset("preset.json")
            elif event.key == pygame.K_l:
                self.load_preset("preset.json")
            elif event.key == pygame.K_ESCAPE:
                self.running = False

    def run(self):
        """Main loop."""
        while self.running:
            self.screen.fill((0, 0, 0))
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = False
                self.handle_event(event)

            # Display instructions and current state
            self.render_text("Press 1 to Select Input", 20, 20)
            self.render_text("Press 2 to Select Output", 20, 60)
            self.render_text("Press S to Save Preset", 20, 100)
            self.render_text("Press L to Load Preset", 20, 140)
            self.render_text("Press ESC to Exit", 20, 180)

            self.render_text(f"Selected Input: {self.selected_input}", 20, 220)
            self.render_text(f"Selected Output: {self.selected_output}", 20, 260)

            pygame.display.flip()

        pygame.quit()


if __name__ == "__main__":
    app = AudioRouterApp()
    app.run()