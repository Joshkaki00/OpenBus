#ifndef CONFIG_H
#define CONFIG_H
#define JUCE_PLUGINHOST_AU 1
#define JUCE_PLUGINHOST_AU3 1

// Fix for SMTG_CPP20 redefinition warnings
#ifndef SMTG_CPP20
    #define SMTG_CPP20 (__cplusplus >= 202002L)
#endif

#endif // CONFIG_H
