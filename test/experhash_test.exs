defmodule ExPerHashTest do
  use ExUnit.Case, async: true

  setup_all do
    {:ok, server} = ExPerHash.start_link

    %{server: server}
  end


  File.cwd!
  |> Path.join("test/image/expected_hashes")
  |> :file.consult
  |> elem(1)

  |> Enum.flat_map(fn(%{filename: filename, hashes: hashes}) ->
       Enum.map hashes, fn({hash_type, expected}) ->
         {hash_type, filename, expected}
       end
     end)

  |> Enum.each(fn({hash_type, filename, expected}) ->
       test "#{hash_type} on #{filename}", %{server: server} do
         {:ok, actual} = ExPerHash.unquote(hash_type)(server, Path.join("test/image", unquote(filename)))

         assert actual == unquote(expected)
       end
     end)

  test "Hamming Distance" do
    assert ExPerHash.hamming_distance(<<0x8cada78f9d8d0e27 :: 64>>, <<0x8cada78f9d8d0e27 :: 64>>) == {:ok, 0}
    assert ExPerHash.hamming_distance(<<0xffffffffffffffff :: 64>>, <<0x0000000000000000 :: 64>>) == {:ok, 64}
    assert ExPerHash.hamming_distance(<<0x9cada78f9d8d0e27 :: 64>>, <<0x8cada78f9d8d0e27 :: 64>>) == {:ok, 1}
    assert ExPerHash.hamming_distance(<<0x8cada7809d8d0e27 :: 64>>, <<0x8cada78f9d8d0e27 :: 64>>) == {:ok, 4}
  end
end

