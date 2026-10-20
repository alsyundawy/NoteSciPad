cask "sparknotepad" do
  arch arm: "arm64", intel: "x86_64"

  version "1.3.0"
  sha256 arm:   "4f85e5055b7d605c45e8f52636a0d4c885faee9c8491c1b3fbc917025fb3eb3c",
         intel: "6b2a0c7bb0b8d5467027b40974ad49ecb281cfc8e0e649171e0c401347ba5e8f"

  url "https://github.com/alsyundawy/SparkNotePad/releases/download/v#{version}/SparkNotePad-v#{version}-#{arch}.dmg",
      verified: "github.com/alsyundawy/SparkNotePad/"
  name "SparkNotePad"
  desc "Lightweight and high-performance native text and source code editor"
  homepage "https://github.com/alsyundawy/SparkNotePad"

  livecheck do
    url :url
    strategy :github_latest
  end

  auto_updates true
  depends_on macos: ">= :monterey"

  app "SparkNotePad.app"
  binary "#{appdir}/SparkNotePad.app/Contents/MacOS/SparkNotePad", target: "sparknotepad"

  zap trash: [
    "~/Library/Application Support/SparkNotePad",
    "~/Library/Caches/io.github.alsyundawy.SparkNotePad",
    "~/Library/Preferences/SparkNotePad",
    "~/Library/Preferences/io.github.alsyundawy.SparkNotePad.plist",
    "~/Library/Saved Application State/io.github.alsyundawy.SparkNotePad.savedState",
  ]
end
