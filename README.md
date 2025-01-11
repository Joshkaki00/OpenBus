# OpenBus

OpenBus is a free, open-source application designed to be a virtual audio mixer, similar to VoiceMeeter. It allows users to manage and route audio from various sources with ease.

## Features

- **Virtual Audio Mixing**: Mix audio from different sources seamlessly.
- **User-Friendly Interface**: Intuitive and easy-to-use interface for all users.
- **Customizable Settings**: Adjust audio settings to suit your needs.
- **Open Source**: Completely free and open-source, with the ability to contribute to the project.

## Installation

### MacOS

1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/openbus.git
    ```
2. Navigate to the project directory:
    ```bash
    cd openbus
    ```
3. Install the required dependencies:
    ```bash
    npm install
    ```
4. Start the application:
    ```bash
    npm start
    ```
5. Install Blackhole for virtual audio routing:
    ```bash
    brew install blackhole-2ch
    ```

### Linux (Debian-based)

1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/openbus.git
    ```
2. Navigate to the project directory:
    ```bash
    cd openbus
    ```
3. Install the required dependencies:
    ```bash
    npm install
    ```
4. Start the application:
    ```bash
    npm start
    ```
5. Ensure PulseAudio and ALSA are installed for virtual audio routing:
    ```bash
    sudo apt-get install pulseaudio alsa-utils
    ```

## Usage

1. Launch OpenBus.
2. Add your audio sources.
3. Configure the audio routing and mixing settings.
4. Enjoy your customized audio experience!

## Contributing

We welcome contributions from the community. To contribute:

1. Fork the repository.
2. Create a new branch:
    ```bash
    git checkout -b feature-branch
    ```
3. Make your changes and commit them:
    ```bash
    git commit -m "Description of changes"
    ```
4. Push to the branch:
    ```bash
    git push origin feature-branch
    ```
5. Create a pull request.

## License

OpenBus is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Contact

For any questions or suggestions, please open an issue on GitHub or contact me at joshkaki00@gmail.com.

## Dependencies

OpenBus uses the following frameworks and libraries:
- **JUCE**: For the audio engine.
- **PyQt5**: For the user interface.
- **ZeroMQ**: For messaging.
- **Nlohmann JSON**: For JSON parsing.
- **Virtual Audio Drivers**: Blackhole for Mac, PulseAudio and ALSA for Linux.

## Disclaimer

**Warning**: OpenBus is currently in development and does not yet have a stable release. Features and functionality may change, and there may be bugs or incomplete features. Use at your own risk.