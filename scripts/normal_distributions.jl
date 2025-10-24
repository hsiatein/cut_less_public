using Plots, Distributions

# 三个不同标准差
σ_values = [0.1, 0.05, 0.02, 0.01, 0.005]
μ = 1.0

# 定义 x 轴范围（覆盖所有分布）
x = 0:0.001:2

# 绘图
plot(title = "Normal Distributions with μ=1 and different cv",
     xlabel = "x", ylabel = "Density", legend = :top)

# 添加三条曲线
for σ in σ_values
    dist = Normal(μ, σ)
    plot!(x, pdf.(dist, x), label = "cv = $(σ)")
end

# 保存或显示
savefig("normal_distributions.png")
println("✅ 已保存为 normal_distributions.png")

# include("normal_distributions.jl")