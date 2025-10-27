using HTTP
using JSON3
using CSV
using DataFrames
using Statistics

name="test_problem3"
json_text = read("../assets/v4/$(name).json", String)
json_data = JSON3.read(json_text)
url = "http://127.0.0.1:6002/cut_less"
modes=[1,2]
results = DataFrame(TotalVolume = Float64[],
                    UtilVolume = Float64[],
                    UtilRate = Float64[],
                    PartsNum = Int32[],
                    SheetsNum = Int32[],
                    CutsNum = Int32[],
                    Mode = Int32[])

for mode in modes
    data = Dict(json_data)
    config = Dict(data[:Config])
    config[:MODE] = mode
    data[:Config] = config
    for i in 1:100
        try
            println("📤 第 $i 次请求中...")
            response = HTTP.post(url; body=JSON3.write(data), headers = ["Content-Type" => "application/json"])
            response_json = JSON3.read(String(response.body))
            metadata = response_json["metadata"]
            push!(results, (
                metadata["TotalVolume"],
                metadata["UtilVolume"],
                metadata["UtilRate"],
                metadata["PartsNum"],
                metadata["SheetsNum"],
                metadata["CutsNum"],
                mode
            ))
            println("✅ 第 $i 次成功: ", metadata)
        catch e
            println("❌ 第 $i 次失败: ", e)
        end
    end
end

grouped = groupby(results, :Mode)

summary_df = combine(grouped,
    :TotalVolume => mean => :TotalVolume_mean,
    :UtilVolume => mean => :UtilVolume_mean,
    :UtilRate => mean => :UtilRate_mean,
    :PartsNum => mean => :PartsNum_mean,
    :SheetsNum => mean => :SheetsNum_mean,
    :CutsNum => mean => :CutsNum_mean
)

println(summary_df)

# include("test_diff.jl")
