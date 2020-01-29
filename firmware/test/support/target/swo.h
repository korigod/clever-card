#ifndef SWO_H_INC
#define SWO_H_INC

int RETARGET_WriteChar(char c);
int RETARGET_ReadChar(void);
void setupSWOForPrint(void);

#endif /* SWO_H_INC */
