cask "notescipad" do
  arch arm: "arm64", intel: "x86_64"

  version "0.9.0"
  sha256 arm:   "4f85e5055b7d605c45e8f52636a0d4c885faee9c8491c1b3fbc917025fb3eb3c",
         intel: "6b2a0c7bb0b8d5467027b40974ad49ecb281cfc8e0e649171e0c401347ba5e8f"

  url "https://github.com/alsyundawy/NoteSciPad/releases/download/v#{version}/NoteSciPad-#{version}-macOS-#{arch}.dmg",
      verified: "github.com/alsyundawy/NoteSciPad/"
  name "NoteSciPad"
  desc "Lightweight and high-performance text and source code editor"
  homepage "https://github.com/alsyundawy/NoteSciPad"

  livecheck do
    url :url
    strategy :github_latest
  end

  auto_updates true
  depends_on macos: ">= :monterey"

  app "NoteSciPad.app"
  binary "#{appdir}/NoteSciPad.app/Contents/MacOS/NoteSciPad", target: "notescipad"

  zap trash: [
    "~/Library/Application Support/NoteSciPad",
    "~/Library/Caches/io.github.alsyundawy.NoteSciPad",
    "~/Library/Preferences/NoteSciPad",
    "~/Library/Preferences/io.github.alsyundawy.NoteSciPad.plist",
    "~/Library/Saved Application State/io.github.alsyundawy.NoteSciPad.savedState",
  ]
end
