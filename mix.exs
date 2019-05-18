defmodule ExPerHash.Mixfile do
  use Mix.Project

  def project do
    [
      app: :experhash,
      name: "ExPerHash",
      version: "0.2.0",
      elixir: "~> 1.1",
      build_embedded: Mix.env == :prod,
      start_permanent: Mix.env == :prod,
      compilers: [:elixir_make] ++ Mix.compilers,
      make_clean: ["clean"],
      package: package(),
      deps: deps()
    ]
  end

  def application do
    [applications: [:logger]]
  end

  defp deps do
    [
      {:elixir_make, "~> 0.5", runtime: false},
      {:dialyxir, "~> 0.5", only: :dev, runtime: false}
    ]
  end

  defp package do
    [
      files: [
        "cpp",
        "lib",
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
