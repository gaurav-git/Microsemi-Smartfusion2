`timescale 1 ns/100 ps
// Version: v12.4 12.900.0.16


module demo_sb_sb_FABOSC_0_OSC(
       XTL,
       RCOSC_25_50MHZ_CCC,
       RCOSC_25_50MHZ_O2F,
       RCOSC_1MHZ_CCC,
       RCOSC_1MHZ_O2F,
       XTLOSC_CCC,
       XTLOSC_O2F
    );
input  XTL;
output RCOSC_25_50MHZ_CCC;
output RCOSC_25_50MHZ_O2F;
output RCOSC_1MHZ_CCC;
output RCOSC_1MHZ_O2F;
output XTLOSC_CCC;
output XTLOSC_O2F;

    wire N_RCOSC_25_50MHZ_CLKINT;
    
    RCOSC_25_50MHZ_FAB I_RCOSC_25_50MHZ_FAB (.A(RCOSC_25_50MHZ_CCC), 
        .CLKOUT(N_RCOSC_25_50MHZ_CLKINT));
    RCOSC_25_50MHZ #( .FREQUENCY(50.0) )  I_RCOSC_25_50MHZ (.CLKOUT(
        RCOSC_25_50MHZ_CCC));
    CLKINT I_RCOSC_25_50MHZ_FAB_CLKINT (.A(N_RCOSC_25_50MHZ_CLKINT), 
        .Y(RCOSC_25_50MHZ_O2F));
    
endmodule
