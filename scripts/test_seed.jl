using HTTP
using JSON3
using CSV
using DataFrames

name="input6"
json_text = read("../assets/v4/$name.json", String)
json_data = JSON3.read(json_text)

url = "http://127.0.0.1:6002/cut_less"

outfile = "$name.csv"

results = DataFrame(TotalVolume = Float64[],
                    UtilVolume = Float64[],
                    UtilRate = Float64[],
                    PartsNum = Int32[],
                    SheetsNum = Int32[],
                    CutsNum = Int32[])

for i in 1:50
    try
        println("📤 第 $i 次请求中...")
        response = HTTP.post(url; body=JSON3.write(json_data), headers = ["Content-Type" => "application/json"])
        response_json = JSON3.read(String(response.body))
        metadata = response_json["metadata"]
        push!(results, (
            metadata["TotalVolume"],
            metadata["UtilVolume"],
            metadata["UtilRate"],
            metadata["PartsNum"],
            metadata["SheetsNum"],
            metadata["CutsNum"]
        ))
        println("✅ 第 $i 次成功: ", metadata)
    catch e
        println("❌ 第 $i 次失败: ", e)
    end
end
file_exists=isfile(outfile)
open(outfile, file_exists ? "a" : "w") do io
    CSV.write(io, results; header = !file_exists, append = true)
end
println("💾 已保存到 ", outfile)

# include("test_seed.jl")