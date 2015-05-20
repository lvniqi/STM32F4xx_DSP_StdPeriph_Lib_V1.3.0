library verilog;
use verilog.vl_types.all;
entity ADC_FIFO is
    generic(
        IDLE            : vl_logic_vector(1 downto 0) := (Hi0, Hi0);
        START           : vl_logic_vector(1 downto 0) := (Hi0, Hi1)
    );
    port(
        clk             : in     vl_logic;
        rd_empty        : in     vl_logic;
        rst_n           : in     vl_logic;
        wr_full         : in     vl_logic;
        rd_enable       : out    vl_logic;
        wr_enable       : out    vl_logic
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of IDLE : constant is 2;
    attribute mti_svvh_generic_type of START : constant is 2;
end ADC_FIFO;
