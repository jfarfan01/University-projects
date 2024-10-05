module multiplier(
  input logic        clk_i,
  input logic        rst_i,
  input logic        start_i,
  input logic [3:0]  a_i,
  input logic [3:0]  b_i,
  output logic       busy_o,
  output logic       valid_o,
  output logic [7:0] result_o
);


enum logic [1:0] {init, calc, done} state_p, state_n;
logic[7:0] result_p, result_n;
logic [1:0] counter_p, counter_n;


//datapath
logic [1:0] result_mux;
always_comb begin 
  case(result_mux)
    2'b00: result_n = result_p;
    2'b01: result_n = 0;
    2'b10: result_n = result_p + (b_i << counter_p);
    default: result_n = result_p;
  endcase
end

logic [1:0] counter_mux;
always_comb begin
  case(counter_mux)
  2'b00: counter_n = counter_p;
  2'b01: counter_n = 0;
  2'b10: counter_n = counter_p + 1;
  default: counter_n = counter_p;
  endcase
end

//output logic
assign result_o = result_p;
assign busy_o = (state_p == calc);
assign valid_o = (state_p == done);

//state machine
always_comb begin
  state_n = state_p;
  result_mux = 2'b00;
  counter_mux = 2'b00;

  case(state_p)
    init:begin
        result_mux = 2'b01;
        counter_mux = 2'b01;

        if(start_i) begin
          state_n = calc;
        end
    end

    calc:begin
      counter_mux = 2'b10;

        if(a_i[counter_p])begin
          result_mux = 2'b10;
        end

        if(counter_p == 3)begin
          state_n = done;
        end

    end

  done:begin
    result_mux = 2'b00;
    counter_mux = 2'b01;

    if(start_i)begin
      result_mux = 2'b01;
      state_n = calc;
    end
    
  end
  endcase
end


//registers
always_ff @( posedge clk_i or posedge rst_i) begin       

if(rst_i)begin
  state_p <= init;
  counter_p = 2'b0;
  result_p = 8'b0;
end                    

else begin
  state_p <= state_n;
  result_p <= result_n;
  counter_p <= counter_n;

end
end
endmodule
