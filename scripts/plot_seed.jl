using CSV
using DataFrames
using Plots
using StatsBase
using Statistics   # ✅ 用于 var()

name = "input7"
df = CSV.read("$(name).csv", DataFrame)

cols = ["TotalVolume", "UtilVolume", "UtilRate", "PartsNum", "SheetsNum", "CutsNum"]

plot_layout = (2, 3)
p = plot(layout = plot_layout, size = (1000, 600))

println("$(name)各字段变异系数：")

for (i, col) in enumerate(cols)
    data = df[!, col]
    μ = mean(data)
    σ = std(data)
    cv = σ / μ
    println(rpad(col, 12), " = ", cv)

    vals = sort(unique(data))
    counts = countmap(data)

    bar!(
        p[i],
        vals,
        [counts[v] for v in vals],
        xlabel = col,
        ylabel = "Count",
        title = col,
        legend = false
    )
end

savefig("$(name).png")
println("✅ 分布图已保存为 metadata_distributions.png")


# include("plot_seed.jl")
