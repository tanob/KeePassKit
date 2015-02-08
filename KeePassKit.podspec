Pod::Spec.new do |s|
  s.name         = "KeePassKit"
  s.version      = "0.0.1"
  s.summary      = "KeePass Database loading, storing and manipulation framework"

  s.description  = <<-DESC
                   KeePass database loading, storing and manipulation framework.
                   It's borrowing a lot of code from [MiniKeePass](https://github.com/MiniKeePass/MiniKeePass) 
                   and [KeePass Database Library](https://github.com/mpowrie/KeePassLib).
                   DESC

  s.homepage     = "https://github.com/tanob/KeePassKit"
  s.license      = "GPL"
  s.authors            = { "Adriano Bonat" => "adrianob@gmail.com", "Michael Starke" => "michael.starke@hicknhack-software.com" }

  s.osx.deployment_target = "10.10"

  s.source       = { :git => "https://github.com/tanob/KeePassKit.git" }

  s.source_files  = "*.h", "Categories", "Core", "Format", "IO", "Protocols", "Utilites"

  s.frameworks = "AppKit"
  s.libraries = "xml2", "z"

  s.xcconfig = { "HEADER_SEARCH_PATHS" => "$(SDKROOT)/usr/include/libxml2" }

  s.dependency "KissXML", "~> 5.0"
end
