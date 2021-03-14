#include <iostream>

#include "neco.h"

void Print(neco::Func continuation, std::string s) {
    std::cout << s << std::endl;
    continuation();
}

template<typename T>
void Print(neco::Func continuation, T i) {
    std::cout << std::to_string(i) << std::endl;
    continuation();
}

NECO_FUNC(LoopPrintDemo, int times) {
    DEFINE_NECO_VAR(
        int i = 0;
        int times;)
    NECO_VAR_REF(times) = times;

    NECO_BEGIN
        if (NECO_VAR_REF(times) <= 0) {
            std::cout << "error" << std::endl;
            NECO_RETURN;
        }
        NECO_WHILE(NECO_VAR_REF(i) < 3)
            NECO_VAR_REF(i)++;
            CALL_CC(Print, NECO_VAR_REF(i));
            NECO_IF(NECO_VAR_REF(i) == 3)
                CALL_CC(Print, std::string{"print finished"});
            NECO_IF_END
        NECO_WHILE_END
    NECO_END
}

int main() {
  // first parameter of LoopPrintDemo is a callback function
  LoopPrintDemo([]{std::cout << "End of Demo\n";}, 3);
  LoopPrintDemo([]{std::cout << "End of Error Demo\n";}, -1);
  return 0;
}
