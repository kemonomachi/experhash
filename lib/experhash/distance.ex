defmodule ExPerHash.Distance do
  use Bitwise, only_operators: true

  @doc """
  Compute hamming distance for two 32-byte binaries.
  This is an optimized version of the generic function below.
  """
  @spec hamming_distance(binary, binary) :: integer
  def hamming_distance(
         <<val1_0::8, val1_1::8, val1_2::8, val1_3::8, val1_4::8, val1_5::8, val1_6::8, val1_7::8,
          val1_8::8, val1_9::8, val1_10::8, val1_11::8, val1_12::8, val1_13::8, val1_14::8, val1_15::8,
          val1_16::8, val1_17::8, val1_18::8, val1_19::8, val1_20::8, val1_21::8, val1_22::8, val1_23::8,
          val1_24::8, val1_25::8, val1_26::8, val1_27::8, val1_28::8, val1_29::8, val1_30::8, val1_31::8>>,
         <<val2_0::8, val2_1::8, val2_2::8, val2_3::8, val2_4::8, val2_5::8, val2_6::8, val2_7::8,
          val2_8::8, val2_9::8, val2_10::8, val2_11::8, val2_12::8, val2_13::8, val2_14::8, val2_15::8,
          val2_16::8, val2_17::8, val2_18::8, val2_19::8, val2_20::8, val2_21::8, val2_22::8, val2_23::8,
          val2_24::8, val2_25::8, val2_26::8, val2_27::8, val2_28::8, val2_29::8, val2_30::8, val2_31::8>>
       ) do
    val1 = val1_0 ^^^ val2_0
    val2 = val1_1 ^^^ val2_1
    val3 = val1_2 ^^^ val2_2
    val4 = val1_3 ^^^ val2_3
    val5 = val1_4 ^^^ val2_4
    val6 = val1_5 ^^^ val2_5
    val7 = val1_6 ^^^ val2_6
    val8 = val1_7 ^^^ val2_7

    val9 = val1_8 ^^^ val2_8
    val10 = val1_9 ^^^ val2_9
    val11 = val1_10 ^^^ val2_10
    val12 = val1_11 ^^^ val2_11
    val13 = val1_12 ^^^ val2_12
    val14 = val1_13 ^^^ val2_13
    val15 = val1_14 ^^^ val2_14
    val16 = val1_15 ^^^ val2_15

    val17 = val1_16 ^^^ val2_16
    val18 = val1_17 ^^^ val2_17
    val19 = val1_18 ^^^ val2_18
    val20 = val1_19 ^^^ val2_19
    val21 = val1_20 ^^^ val2_20
    val22 = val1_21 ^^^ val2_21
    val23 = val1_22 ^^^ val2_22
    val24 = val1_23 ^^^ val2_23

    val25 = val1_24 ^^^ val2_24
    val26 = val1_25 ^^^ val2_25
    val27 = val1_26 ^^^ val2_26
    val28 = val1_27 ^^^ val2_27
    val29 = val1_28 ^^^ val2_28
    val30 = val1_29 ^^^ val2_29
    val31 = val1_30 ^^^ val2_30
    val32 = val1_31 ^^^ val2_31

    %{
      ^val1 => distance1,
      ^val2 => distance2,
      ^val3 => distance3,
      ^val4 => distance4,
      ^val5 => distance5,
      ^val6 => distance6,
      ^val7 => distance7,
      ^val8 => distance8,

      ^val9 => distance9,
      ^val10 => distance10,
      ^val11 => distance11,
      ^val12 => distance12,
      ^val13 => distance13,
      ^val14 => distance14,
      ^val15 => distance15,
      ^val16 => distance16,

      ^val17 => distance17,
      ^val18 => distance18,
      ^val19 => distance19,
      ^val20 => distance20,
      ^val21 => distance21,
      ^val22 => distance22,
      ^val23 => distance23,
      ^val24 => distance24,

      ^val25 => distance25,
      ^val26 => distance26,
      ^val27 => distance27,
      ^val28 => distance28,
      ^val29 => distance29,
      ^val30 => distance30,
      ^val31 => distance31,
      ^val32 => distance32
    } = lookup_map()

    {:ok,
     distance1 + distance2 + distance3 + distance4 + distance5 + distance6 + distance7 + distance8 +
     distance9 + distance10 + distance11 + distance12 + distance13 + distance14 + distance15 + distance16 +
     distance17 + distance18 + distance19 + distance20 + distance21 + distance22 + distance23 + distance24 +
     distance25 + distance26 + distance27 + distance28 + distance29 + distance30 + distance31 + distance32
   }
  end

  @doc """
  Compute hamming distance for two binaries of equal size.
  """
  @spec hamming_distance(binary, binary) :: integer
  def hamming_distance(hash1, hash2) do
    {:ok, hamming_value_distance(:binary.bin_to_list(hash1), :binary.bin_to_list(hash2), 0)}
  end

  defp hamming_value_distance([], [], sum), do: sum

  defp hamming_value_distance([val1 | tail1], [val2 | tail2], sum) do
    value = val1 ^^^ val2

    %{^value => distance} = lookup_map()

    distance + hamming_value_distance(tail1, tail2, sum)
  end

  defp lookup_map() do
    %{
      0 => 0,
      1 => 1,
      2 => 1,
      3 => 2,
      4 => 1,
      5 => 2,
      6 => 2,
      7 => 3,
      8 => 1,
      9 => 2,
      10 => 2,
      11 => 3,
      12 => 2,
      13 => 3,
      14 => 3,
      15 => 4,
      16 => 1,
      17 => 2,
      18 => 2,
      19 => 3,
      20 => 2,
      21 => 3,
      22 => 3,
      23 => 4,
      24 => 2,
      25 => 3,
      26 => 3,
      27 => 4,
      28 => 3,
      29 => 4,
      30 => 4,
      31 => 5,
      32 => 1,
      33 => 2,
      34 => 2,
      35 => 3,
      36 => 2,
      37 => 3,
      38 => 3,
      39 => 4,
      40 => 2,
      41 => 3,
      42 => 3,
      43 => 4,
      44 => 3,
      45 => 4,
      46 => 4,
      47 => 5,
      48 => 2,
      49 => 3,
      50 => 3,
      51 => 4,
      52 => 3,
      53 => 4,
      54 => 4,
      55 => 5,
      56 => 3,
      57 => 4,
      58 => 4,
      59 => 5,
      60 => 4,
      61 => 5,
      62 => 5,
      63 => 6,
      64 => 1,
      65 => 2,
      66 => 2,
      67 => 3,
      68 => 2,
      69 => 3,
      70 => 3,
      71 => 4,
      72 => 2,
      73 => 3,
      74 => 3,
      75 => 4,
      76 => 3,
      77 => 4,
      78 => 4,
      79 => 5,
      80 => 2,
      81 => 3,
      82 => 3,
      83 => 4,
      84 => 3,
      85 => 4,
      86 => 4,
      87 => 5,
      88 => 3,
      89 => 4,
      90 => 4,
      91 => 5,
      92 => 4,
      93 => 5,
      94 => 5,
      95 => 6,
      96 => 2,
      97 => 3,
      98 => 3,
      99 => 4,
      100 => 3,
      101 => 4,
      102 => 4,
      103 => 5,
      104 => 3,
      105 => 4,
      106 => 4,
      107 => 5,
      108 => 4,
      109 => 5,
      110 => 5,
      111 => 6,
      112 => 3,
      113 => 4,
      114 => 4,
      115 => 5,
      116 => 4,
      117 => 5,
      118 => 5,
      119 => 6,
      120 => 4,
      121 => 5,
      122 => 5,
      123 => 6,
      124 => 5,
      125 => 6,
      126 => 6,
      127 => 7,
      128 => 1,
      129 => 2,
      130 => 2,
      131 => 3,
      132 => 2,
      133 => 3,
      134 => 3,
      135 => 4,
      136 => 2,
      137 => 3,
      138 => 3,
      139 => 4,
      140 => 3,
      141 => 4,
      142 => 4,
      143 => 5,
      144 => 2,
      145 => 3,
      146 => 3,
      147 => 4,
      148 => 3,
      149 => 4,
      150 => 4,
      151 => 5,
      152 => 3,
      153 => 4,
      154 => 4,
      155 => 5,
      156 => 4,
      157 => 5,
      158 => 5,
      159 => 6,
      160 => 2,
      161 => 3,
      162 => 3,
      163 => 4,
      164 => 3,
      165 => 4,
      166 => 4,
      167 => 5,
      168 => 3,
      169 => 4,
      170 => 4,
      171 => 5,
      172 => 4,
      173 => 5,
      174 => 5,
      175 => 6,
      176 => 3,
      177 => 4,
      178 => 4,
      179 => 5,
      180 => 4,
      181 => 5,
      182 => 5,
      183 => 6,
      184 => 4,
      185 => 5,
      186 => 5,
      187 => 6,
      188 => 5,
      189 => 6,
      190 => 6,
      191 => 7,
      192 => 2,
      193 => 3,
      194 => 3,
      195 => 4,
      196 => 3,
      197 => 4,
      198 => 4,
      199 => 5,
      200 => 3,
      201 => 4,
      202 => 4,
      203 => 5,
      204 => 4,
      205 => 5,
      206 => 5,
      207 => 6,
      208 => 3,
      209 => 4,
      210 => 4,
      211 => 5,
      212 => 4,
      213 => 5,
      214 => 5,
      215 => 6,
      216 => 4,
      217 => 5,
      218 => 5,
      219 => 6,
      220 => 5,
      221 => 6,
      222 => 6,
      223 => 7,
      224 => 3,
      225 => 4,
      226 => 4,
      227 => 5,
      228 => 4,
      229 => 5,
      230 => 5,
      231 => 6,
      232 => 4,
      233 => 5,
      234 => 5,
      235 => 6,
      236 => 5,
      237 => 6,
      238 => 6,
      239 => 7,
      240 => 4,
      241 => 5,
      242 => 5,
      243 => 6,
      244 => 5,
      245 => 6,
      246 => 6,
      247 => 7,
      248 => 5,
      249 => 6,
      250 => 6,
      251 => 7,
      252 => 6,
      253 => 7,
      254 => 7,
      255 => 8
    }
  end
end
