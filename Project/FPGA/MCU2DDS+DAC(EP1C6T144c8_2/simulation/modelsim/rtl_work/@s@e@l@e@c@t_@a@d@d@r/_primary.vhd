library verilog;
use verilog.vl_types.all;
entity SELECT_ADDR is
    port(
        ADDR            : in     vl_logic_vector(18 downto 0);
        RAM_DDS         : out    vl_logic;
        FREWL           : out    vl_logic;
        FREWH           : out    vl_logic;
        FIFO_ADIN       : out    vl_logic;
        read_en         : out    vl_logic
    );
end SELECT_ADDR;
