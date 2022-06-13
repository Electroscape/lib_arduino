

/**
 * @brief 
 * @param rcvd 
 * @param slaveNo 
 */

/*
void STB::cmdInterpreter(int slaveNo) {
    defaultOled.print("rcvd cmd from "); defaultOled.println(slaveNo);       
    defaultOled.println(rcvd);     

    int lineCnt = 0;
    char* line = strtok(rcvd, "\n"); 

    while (line != NULL) {
        lineCnt++;
        line = strtok(NULL, "\n");
    }

    line = rcvd;
    int lineLenght = 0;

    for (int lineNo = 0; lineNo < lineCnt; lineNo++) {

        lineLenght = strlen(line);
        if (strncmp(line, relayKeyword, 6) != 0) {
            line += lineLenght + 1;
            continue;
        }

        int i = 0;
        int values[2] = {0,0};
        char* splits = strtok(line, delimiter);
        // need to get rid of the keyword itself
        splits = strtok(NULL, delimiter);

        while (splits && i < 2) {
            values[i++] = atoi(splits);
            splits = strtok(NULL, delimiter);
        }

        if (i==2) {
            // add a safety check here
            defaultOled.print("values are: "); 
            defaultOled.print(values[0]);
            defaultOled.print("  ");
            defaultOled.println(values[1]);
            motherRelay.digitalWrite(values[0], values[1]);
        }  

        line += lineLenght + 1;

    }
    
}

*/