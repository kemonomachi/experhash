defmodule ExPerHash.Mixfile do
  use Mix.Project

  def project do
    [
      app: :experhash,
      name: "ExPerHash",
      version: "0.1.1",
      elixir: "~> 1.0",
      build_embedded: Mix.env == :prod,
      start_permanent: Mix.env == :prod,
      compilers: [:make, :elixir, :app],
      package: package(),
      aliases: aliases(),
      deps: deps()
    ]
  end

  def application do
    [applications: [:logger]]
  end

  defp aliases do
    [clean: ["clean", "clean.make"]]
  end

  defp deps do
    []
  end

  defp package do
    [
      files: [
        "cpp",
        "lib",
        "priv",
        "mix.exs",
        "README*",
        "LICENSE*",
      ],
      maintainers: ["Ookami Kenrou <ookamikenrou@gmail.com>"],
      licenses: ["WTFPL"],
      links: %{"GitHub" => "https://github.com/kemonomachi/experhash"}
    ]
  end
end

defmodule Mix.Tasks.Compile.Make do
  @shortdoc "Compile C++ code"
  use Mix.Task

  def run(_) do
    {result, _error_code} = System.cmd("make", [], stderr_to_stdout: true)
    Mix.shell.info result

    :ok
  end
end

defmodule Mix.Tasks.Clean.Make do
  @shortdoc "Delete generated C++ files"
  use Mix.Task

  def run(_) do
    {result, _error_code} = System.cmd("make", ["clean"], stderr_to_stdout: true)
    Mix.shell.info result

    :ok
  end
end
