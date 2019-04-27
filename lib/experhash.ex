defmodule ExPerHash do
  use GenServer

  @timeout 10_000

  @type error :: {:error, {atom, atom}}

  @doc """
  Start an ExPerHash server process linked to the current process.
  """
  @spec start_link() :: GenServer.on_start
  def start_link() do
    GenServer.start_link __MODULE__, []
  end

  @doc """
  Start an ExPerHash server process linked to the current process, registered
  as `name`.
  """
  @spec start_link(GenServer.name) :: GenServer.on_start
  def start_link(name) do
    GenServer.start_link __MODULE__, [], name: name
  end

  Enum.each [8, 16, 32, 64], fn(size) ->
    bit_size = size * size

    Enum.each [:a, :d], fn(type) ->
      function_name = :"#{type}_hash_#{bit_size}"

      @doc """
      Compute the #{bit_size}-bit #{type}Hash for the image named `filename`.
      """
      @spec unquote(function_name)(GenServer.server, String.t) :: {:ok, <<_ :: unquote(bit_size)>>} | error
      def unquote(function_name)(server, filename) do
        GenServer.call server, {:command, {:hash, unquote(type), filename, unquote(size)}}, @timeout
      end
    end

    function_name = :"dd_hash_#{2 * bit_size}"
    @doc """
    Compute the #{2 * bit_size}-bit ddHash for the image named `filename`.
    """
    @spec unquote(function_name)(GenServer.server, String.t) :: {:ok, <<_ :: unquote(2 * bit_size)>>} | error
    def unquote(function_name)(server, filename) do
      GenServer.call server, {:command, {:hash, :dd, filename, unquote(size)}}, @timeout
    end
  end

  @doc """
  Calculate the 64-bit aHash for the image named `filename`.
  """
  @deprecated "Use a_hash_64/2 instead"
  @spec a_hash(GenServer.server, String.t) :: {:ok, <<_ :: 64>>} | error
  defdelegate a_hash(server, filename), to: __MODULE__, as: :a_hash_64

  @doc """
  Calculate the 64-bit dHash for the image named `filename`.
  """
  @deprecated "Use d_hash_64/2 instead"
  @spec d_hash(GenServer.server, String.t) :: {:ok, <<_ :: 64>>} | error
  defdelegate d_hash(server, filename), to: __MODULE__, as: :d_hash_64

  @doc """
  Calculate the 128-bit ddHash for the image named `filename`.
  """
  @deprecated "Use dd_hash_128/2 instead"
  @spec dd_hash(GenServer.server, String.t) :: {:ok, <<_ :: 128>>} | error
  defdelegate dd_hash(server, filename), to: __MODULE__, as: :dd_hash_128

  @doc """
  Calculate the Hamming distance between two hashes.
  """
  @spec hamming_distance(GenServer.server, binary, binary) :: {:ok, non_neg_integer} | error
  def hamming_distance(_server, hash1, hash2) do
    # call optimized Elixir version
    ExPerHash.Distance.hamming_distance(hash1, hash2)
  end


  defmodule State do
    defstruct port: nil
  end

  def init(_args) do
    executable = Path.join(:code.priv_dir(:experhash), "experhash_port")
    port = Port.open {:spawn, executable}, [{:packet, 4}, :binary]

    {:ok, %State{port: port}}
  end

  def handle_call({:command, command}, _from, state) do
    send_command state.port, command

    {:reply, receive_response(state.port), state}
  end

  defp send_command(port, command) do
    true = Port.command port, :erlang.term_to_binary(command)
  end

  defp receive_response(port) do
    receive do
      {^port, {:data, data}} ->
        :erlang.binary_to_term data
    after
      @timeout ->
        {:error, :port_timeout}
    end
  end
end
