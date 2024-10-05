module gcd (
  input  logic        clk_i,
  input  logic        rst_i,
  input  logic        start_i,
  input  logic [31:0] a_i,
  input  logic [31:0] b_i,
  output logic        busy_o,
  output logic        valid_o,
  output logic [31:0] result_o
);


enum logic [3:0] {init, check_a, check_b, first_while, second_while, inner_while, swap, iteration_end, done}state_n, state_p;

//registers
logic [31:0] a_n, a_p, b_n, b_p, result_n, result_p;
logic [4:0] k_n, k_p;

//control signals
enum logic [1:0] {A,B, CURRENT_RESULT, SHIFTED_A } result_control;
enum logic [1:0] {INPUT_A, A_SHIFT, CURRENT_A, SWAP_A} a_control;
enum logic [2:0] {INPUT_B, B_SHIFT, CURRENT_B, SWAP_B, B_MINUS_A} b_control;
enum logic [1:0] {ZERO, CURRENT_K, PLUS_ONE} k_control;


//datapath
always_comb begin
        case(a_control)
            INPUT_A: a_n = a_i;
            A_SHIFT: a_n = a_p >> 1;
            CURRENT_A: a_n = a_p;
            SWAP_A: a_n = b_p;
        endcase
  
end

always_comb begin
        case(b_control)
            INPUT_B: b_n = b_i;
            B_SHIFT: b_n = b_p >> 1;
            CURRENT_B: b_n = b_p;
            SWAP_B: b_n = a_p;
            B_MINUS_A: b_n = b_p - a_p;
            default: b_n = b_p;
        endcase
  
end

always_comb begin
        case(k_control)
                ZERO : k_n = 0;
                CURRENT_K: k_n = k_p;
                PLUS_ONE: k_n = k_p + 1;
                default: k_n = k_p;
        endcase
  
end

always_comb begin
        case(result_control)
                A: result_n = a_p;
                B: result_n = b_p;
                CURRENT_RESULT: result_n = result_p;
                SHIFTED_A: result_n = a_p << k_p;
        endcase

end

//output logic
assign result_o = result_p;
assign busy_o = (state_p == check_a ||state_p == check_b|| state_p == first_while
|| state_p == second_while || state_p == swap
|| state_p == inner_while|| state_p == iteration_end);
assign valid_o = (state_p == done);

//state machine
always_comb begin
	state_n = state_p;
	result_control = CURRENT_RESULT;
	a_control =  CURRENT_A;
	b_control = CURRENT_B;
	k_control = CURRENT_K;

	case(state_p)

		init: begin
			a_control = INPUT_A;
			b_control = INPUT_B;
			if(start_i)begin
				state_n = check_a;
			end
		end

		check_a: begin
			state_n = check_b;
			if(a_p == 0)begin
				result_control = B;
				state_n = done;
			end
		end

		check_b:begin
			state_n = first_while;
			if(b_p == 0)begin
				result_control = A;
				state_n = done;
			end
			else begin
			k_control = ZERO;
			end
		end

		first_while: begin
			if(((a_p | b_p) & 1) == 0) begin
			a_control = A_SHIFT;
			b_control = B_SHIFT;
			k_control = PLUS_ONE;
			end
			else begin
				state_n = second_while;
			end
		end

		second_while: begin
			if((a_p & 1) == 0)begin
			a_control = A_SHIFT;
			end
			else begin
				state_n = inner_while;
			end
		end

		inner_while:begin
			if((b_p & 1) != 0)begin
				state_n = swap;
			end
			else begin 
				b_control = B_SHIFT;
			end;

		end

		swap:begin
			if(a_p >b_p)begin
				a_control = SWAP_A;
				b_control = SWAP_B;
			end
			state_n = iteration_end;
		end

		iteration_end:begin
			b_control = B_MINUS_A;
			if(b_p != 0)begin
				state_n = inner_while;
			end
			if(b_n == 0) begin
				result_control = SHIFTED_A;
				state_n = done;
			end
		end

		done:begin
			state_n = init;
		end
	endcase
end

//registers
always_ff @(posedge clk_i or posedge rst_i) begin 
	if (rst_i) begin
		state_p <= init;
		a_p <= 32'b0;
		b_p <= 32'b0;
		k_p <= 5'b0;
		result_p <= 32'b0;
	end

	else begin
		state_p <= state_n;
				a_p <= a_n;
				b_p <= b_n;
				k_p <= k_n;
				result_p <= result_n;
	end	
end
endmodule