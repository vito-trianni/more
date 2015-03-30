set(neural_networks_HEADERS
  neural_network_controller.h
  singlelayer_feedforward_controller.h
  ctrnn_perceptron_controller.h
  ctrnn_multilayer_controller.h
)

set(neural_networks_IMPLEMENTATIONS
  neural_network_controller.cpp
  perceptron_controller.cpp
  singlelayer_feedforward_controller.cpp
  ctrnn_perceptron_controller.cpp
  ctrnn_multilayer_controller.cpp
)


add_library(neural_networks SHARED ${neural_networks_IMPLEMENTATIONS})

