defmodule ExPerHashTest do
  use ExUnit.Case

  setup_all do
    #Testfile download: http://danbooru.donmai.us/posts/167739
    testfile = Path.join File.cwd!, "test/image/264f94fb2b7d5718a43534830aa74ff2.jpg"

    if not File.exists?(testfile) do
      {:error, "Testfile not found"}
    else
      {:ok, srv} = ExPerHash.start_link

      {:ok, %{server: srv, image: testfile}}
    end
  end


  test "aHash", %{server: server, image: image} do
    {:ok, hash} = ExPerHash.a_hash server, image

    expected = <<0xfffff3c3c7c38300 :: 64>>

    assert hash == expected
  end

  test "dHash", %{server: server, image: image} do
    {:ok, hash} = ExPerHash.d_hash server, image

    expected = <<0x8cada78f9d8d0e27 :: 64>>

    assert hash == expected
  end

  test "ddHash", %{server: server, image: image} do
    {:ok, hash} = ExPerHash.dd_hash server, image

    expected = <<0x8cada78f9d8d0e27 :: 64, 0x4d09038717311959 :: 64>>

    assert hash == expected
  end

  test "Hamming Distance", %{server: server} do
    assert ExPerHash.hamming_distance(server, <<0x8cada78f9d8d0e27 :: 64>>, <<0x8cada78f9d8d0e27 :: 64>>) == {:ok, 0}
    assert ExPerHash.hamming_distance(server, <<0xffffffffffffffff :: 64>>, <<0x0000000000000000 :: 64>>) == {:ok, 64}
    assert ExPerHash.hamming_distance(server, <<0x9cada78f9d8d0e27 :: 64>>, <<0x8cada78f9d8d0e27 :: 64>>) == {:ok, 1}
    assert ExPerHash.hamming_distance(server, <<0x8cada7809d8d0e27 :: 64>>, <<0x8cada78f9d8d0e27 :: 64>>) == {:ok, 4}
  end
end

