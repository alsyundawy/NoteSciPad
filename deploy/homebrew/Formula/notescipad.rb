class Notescipad < Formula
  desc "Lightweight and high-performance text and source code editor"
  homepage "https://github.com/alsyundawy/NoteSciPad"
  url "https://github.com/alsyundawy/NoteSciPad/archive/refs/tags/v0.9.0.tar.gz"
  sha256 "0000000000000000000000000000000000000000000000000000000000000000"
  license "GPL-3.0-or-later"
  head "https://github.com/alsyundawy/NoteSciPad.git", branch: "main"

  depends_on "cmake" => :build
  depends_on "ninja" => :build
  depends_on "qt@6"
  depends_on "uchardet"

  def install
    system "cmake", "-S", ".", "-B", "build", "-G", "Ninja", *std_cmake_args
    system "cmake", "--build", "build"
    system "cmake", "--install", "build"
  end

  test do
    assert_match "NoteSciPad", shell_output("#{bin}/notescipad --help", 0)
  end
end
