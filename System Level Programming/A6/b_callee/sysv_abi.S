fsize:
.global 	fsize

    // TODO: Implement this function in x86 64-bit assembly using the System V AMD64 ABI
    //       See the wiki for the functionality you need to implement and some documents about
    //       memory organization in GNU assembly (gas)
    //       Hints: 
    //          * the struct for fstat needs to be 144 bytes long
    //          * the size info is stored at an offset of 48 bytes
    //          * you may need to Load the Effective Adress of your memory     
    .bss
        fstat_memory: .zero 144

    


     // Remove this when you start your implementation
